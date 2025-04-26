/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:04 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 11:16:23 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Free a single redirection node and its linked list */
void	free_redir(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = temp;
	}
}

/* Free a single command node and its linked list */
void	free_cmd(t_cmd *cmd)
{
	t_cmd	*temp;
	int		i;

	if (!cmd)
	{
		return ;
	}
	while (cmd)
	{
		temp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		if (cmd->redirs)
		{
			free_redir(cmd->redirs);
		}
		free(cmd);
		cmd = temp;
	}
}

/* Free an array of tokens */
void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

/* Free the environment variables array */
void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

/* Free the entire shell structure */
void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd)
		free_cmd(shell->cmd);
	free(shell);
}
