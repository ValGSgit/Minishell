/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:04 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 19:30:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_redir(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir->next;
		if (redir->file && redir->type == REDIR_HEREDOC)
		{
			if (access(redir->file, F_OK) == 0)
				unlink(redir->file);
		}
		if (redir->file)
			free(redir->file);
		if (redir->prefile)
			free(redir->prefile);
		safe_free(redir);
		redir = temp;
	}
}

void	close_cmd_fds(t_cmd *cmd)
{
	if (!cmd)
		return ;
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
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*temp;
	int		i;

	if (!cmd)
		return ;
	while (cmd)
	{
		temp = cmd->next;
		close_cmd_fds(cmd);
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				safe_free(cmd->args[i++]);
			safe_free(cmd->args);
		}
		if (cmd->redirs)
			free_redir(cmd->redirs);
		if (cmd->in_file)
			safe_free(cmd->in_file);
		if (cmd->out_file)
			safe_free(cmd->out_file);
		safe_free(cmd);
		cmd = temp;
	}
}
