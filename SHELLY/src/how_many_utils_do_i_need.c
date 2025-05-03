/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   how_many_utils_do_i_need.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:52:52 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 19:53:29 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* void	cleanup_cmd_heredocs(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*next;
	t_redir	*prev;

	redir = cmd->redirs;
	prev = NULL;
	while (redir && redir->type == REDIR_HEREDOC)
	{
		next = redir->next;
		if (redir->file)
		{
			if (access(redir->file, F_OK) == 0 && unlink(redir->file) == -1)
				perror("unlink");
			safe_free(redir->file);
		}
		if (redir->prefile)
			safe_free(redir->prefile);
		free(redir);
		redir = next;
	}
	if (prev)
		prev->next = redir;
	else
		cmd->redirs = redir;
} */

void	cleanup_cmd_heredocs(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*next;
	t_redir	*prev;

	redir = cmd->redirs;
	prev = NULL;
	while (redir && redir->type == REDIR_HEREDOC)
	{
		next = redir->next;
		if (redir->file)
		{
			if (access(redir->file, F_OK) == 0 && unlink(redir->file) == -1)
				perror("unlink");
			safe_free(redir->file);
		}
		if (redir->prefile)
			safe_free(redir->prefile);
		free(redir);
		if (prev)
			prev->next = next;
		redir = next; 
	}
	if (!prev)
		cmd->redirs = redir;
}


void	cleanup_heredocs(t_shell *shell)
{
	t_cmd	*node;

	if (!shell || !shell->cmd)
		return ;
	node = shell->cmd;
	while (node)
	{
		cleanup_cmd_heredocs(node);
		node = node->next;
	}
}
