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

/**
 * Free a single redirection node and its linked list
 */
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

void close_cmd_fds(t_cmd *cmd)
{
    if (!cmd)
        return;
    
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

/**
 * Free a single command node and its linked list
 */
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

/**
 * Free an array of tokens
 */
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

/**
 * Free the environment variables array
 */
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

/**
 * Free the entire shell structure
 */
void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd)
		free_cmd(shell->cmd);
	// The shell itself should be on the stack, so we don't free it
}
