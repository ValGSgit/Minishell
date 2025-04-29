/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:26:40 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/29 22:22:16 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	apply_redirection(t_cmd *cmd, bool fork)
{
	t_cmd	*node;
	bool	error;

	error = false;
	node = cmd;
	if (cmd->redirs->file && cmd->redirs->file[0] == '\0')
	{
		cmd->shell->exit_status = 1;
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd->redirs->prefile, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		if (fork)
		{
			close_cmd_fds(cmd);
			exit(1);
		}
		return;
	}
	while (node->redirs && !error)
	{
		if (node->redirs->type == REDIR_IN)
			error = handle_redirection_in(node, fork);
		else if (node->redirs->type == REDIR_OUT)
			error = handle_redirection_out(0, node, fork);
		else if (node->redirs->type == REDIR_APPEND)
			error = handle_redirection_out(1, node, fork);
		else if (node->redirs->type == REDIR_HEREDOC)
			error = handle_redirection_in(node, fork);
		
		if (!error)
			node->redirs = node->redirs->next;
	}
	
	if (error && fork)
		exit(1);
}

bool	handle_redirection_in(t_cmd *cmd, bool fork)
{
	int fd;

	fd = open(cmd->redirs->file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->redirs->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cmd->shell->exit_status = 1;
		if (fork)
		{
			close_cmd_fds(cmd); // Close any open FDs before exit
			exit(1); //maybe return instead
		}
		return true;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return false;
}

bool	handle_redirection_out(int append, t_cmd *cmd, bool fork)
{
	int fd;

	if (append)
		fd = open(cmd->redirs->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fd = open(cmd->redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->redirs->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cmd->shell->exit_status = 1;
		if (fork)
		{
			close_cmd_fds(cmd); // Close any open FDs before exit
			exit(1);
		}
		return true;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return false;
}

