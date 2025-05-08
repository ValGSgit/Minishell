/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:26:40 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 12:28:15 by vagarcia         ###   ########.fr       */
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
			forked_exit(1, cmd);
		}
		return (true);
	}
	return (false);
}

bool	apply_redirection(t_cmd *cmd, bool fork)
{
	t_redir	*node;
	bool	error;

	error = false;
	node = cmd->redirs;
	while (node && !error)
	{
		if (check_ambiguous_redirect(node, cmd, fork))
			return (true);
		if (node->type == REDIR_IN || node->type == REDIR_HEREDOC)
			error = handle_redirection_in(node, cmd, fork);
		else if (node->type == REDIR_OUT)
			error = handle_redirection_out(0, node, cmd, fork);
		else if (node->type == REDIR_APPEND)
			error = handle_redirection_out(1, node, cmd, fork);
		node = node->next;
	}
	if (error && fork)
		forked_exit(1, cmd);
	return (error);
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
			forked_exit(1, cmd);
		}
		return (true);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
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
			forked_exit(1, cmd);
		}
		return (true);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (false);
}
