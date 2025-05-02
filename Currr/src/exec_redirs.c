/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:26:40 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/02 17:46:46 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	check_ambiguous_redirect(t_redir *node, t_cmd *cmd, bool fork)
{
	if (node->file && node->file[0] == '\0')
	{
		cmd->shell->exit_status = 1;
		print_error_message("minishell: ", node->prefile, \
			": ambiguous redirect\n");
		if (fork)
		{
			close_cmd_fds(cmd);
			exit(1);
		}
		return (true);
	}
	return (false);
}

void	apply_redirection(t_cmd *cmd, bool fork)
{
	t_redir	*node;
	bool	error;

	error = false;
	node = cmd->redirs;
	while (node && !error)
	{
		if (check_ambiguous_redirect(node, cmd, fork))
			return ;
		if (node->type == REDIR_IN || node->type == REDIR_HEREDOC)
			error = handle_redirection_in(node, cmd, fork);
		else if (node->type == REDIR_OUT)
			error = handle_redirection_out(0, node, cmd, fork);
		else if (node->type == REDIR_APPEND)
			error = handle_redirection_out(1, node, cmd, fork);
		node = node->next;
	}
	if (error && fork)
		exit(1);
}

bool	handle_redirection_in(t_redir *redir, t_cmd *cmd, bool fork)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		print_error_message("minishell: ", redir->file, ": ");
		print_error_message(NULL, strerror(errno), "\n");
		cmd->shell->exit_status = 1;
		if (fork)
		{
			close_cmd_fds(cmd);
			cleanup_shell(cmd->shell);
			exit(1);
		}
		return (true);
	}
	if (cmd->in_fd >= 0 && cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	cmd->in_fd = fd;
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		cmd->in_fd = -1;
		print_error_message("minishell: ", "dup2 failed", "\n");
		if (fork)
		{
			close_cmd_fds(cmd);
			cleanup_shell(cmd->shell);
			exit(1);
		}
		return (true);
	}
	return (false);
}

bool	handle_redirection_out(int append, t_redir *redir, \
		t_cmd *cmd, bool fork)
{
	int	fd;

	if (append)
		fd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_error_message("minishell: ", redir->file, ": ");
		print_error_message(NULL, strerror(errno), "\n");
		cmd->shell->exit_status = 1;
		if (fork)
		{
			close_cmd_fds(cmd);
			cleanup_shell(cmd->shell);
			exit(1);
		}
		return (true);
	}
	if (cmd->out_fd >= 0 && cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
	cmd->out_fd = fd;
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		cmd->out_fd = -1;
		print_error_message("minishell: ", "dup2 failed", "\n");
		if (fork)
		{
			close_cmd_fds(cmd);
			cleanup_shell(cmd->shell);
			exit(1);
		}
		return (true);
	}
	return (false);
}

/**
 * Reset redirections after command execution to avoid file descriptor leaks
 * Should be called after a command with redirections finishes executing
 */
void	reset_redirections(t_cmd *cmd)
{
	if (cmd->in_fd >= 0 && cmd->in_fd != STDIN_FILENO)
	{
		close(cmd->in_fd);
		cmd->in_fd = -1;
	}
	if (cmd->out_fd >= 0 && cmd->out_fd != STDOUT_FILENO)
	{
		close(cmd->out_fd);
		cmd->out_fd = -1;
	}
	if (isatty(STDIN_FILENO) == 0)
		dup2(STDERR_FILENO, STDIN_FILENO);
	if (isatty(STDOUT_FILENO) == 0)
		dup2(STDERR_FILENO, STDOUT_FILENO);
}
