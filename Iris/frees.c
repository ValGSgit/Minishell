/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:04 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/27 11:15:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_redir(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir->next;
		if (redir->file)
			safe_free(redir->file);
		if (redir->prefile)
			safe_free(redir->prefile);
		safe_free(redir);
		redir = temp;
	}
}

void	close_cmd_fds(t_cmd *cmd)
{
	t_redir	*redir;

	if (!cmd)
		return ;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC && redir->file)
		{
			if (access(redir->file, F_OK) == 0)
				unlink(redir->file);
		}
		redir = redir->next;
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
		safe_free(cmd);
		cmd = temp;
	}
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		safe_free(tokens[i]);
		i++;
	}
	safe_free(tokens);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		safe_free(env[i]);
		i++;
	}
	safe_free(env);
}


