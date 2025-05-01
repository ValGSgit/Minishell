/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:26:40 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 20:17:52 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	check_ambiguous_redirect(t_redir *node, t_cmd *cmd, bool fork)
{
	if (node->file && node->file[0] == '\0')
	{
		cmd->shell->exit_status = 1;
		print_error_message("minishell: ", node->prefile, ": ambiguous redirect\n");
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
	t_redir	*redir;
	bool	error;

	error = false;
	redir = cmd->redirs;
	while (redir && !error)
	{
		if (check_ambiguous_redirect(redir, cmd, fork))
			return ;
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
			error = handle_redirection_in(cmd, redir, fork);
		else if (redir->type == REDIR_OUT)
			error = handle_redirection_out(cmd, 0, redir, fork);
		else if (redir->type == REDIR_APPEND)
			error = handle_redirection_out(cmd, 1, redir, fork);
		redir = redir->next;
	}
	if (error && fork)
		exit(1);
}

bool	handle_redirection_in(t_cmd *cmd, t_redir *redir, bool fork)
{
	int fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		print_error_message("minishell: ", redir->file, ": ");
		print_error_message(NULL, strerror(errno), "\n");
		cmd->shell->exit_status = 1;
		if (fork)
		{
			close_cmd_fds(cmd);
			exit(1);
		}
		return (true);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (false);
}

bool	handle_redirection_out(t_cmd *cmd, int append,t_redir *redir, bool fork)
{
	int fd;

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
			exit(1);
		}
		return (true);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (false);
}

