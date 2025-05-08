/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:58:09 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/03 18:58:09 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_redir	*create_redir_node_struct(int type, char *file, t_cmd *cmd)
{
	t_redir	*node;

	if (!file)
		return (NULL);
	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->type = type;
	node->prefile = ft_strdup(file);
	if (!node->prefile)
	{
		free(node);
		return (NULL);
	}
	node->file = process_argument(file, cmd->shell);
	if (!node->file)
	{
		free(node->prefile);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	create_redir_node(t_cmd *cmd, int type, char *file)
{
	t_redir		*node;
	t_redir		*tmp;

	if (!cmd)
		return ;
	node = create_redir_node_struct(type, file, cmd);
	if (!node)
		return ;
	if (!cmd->redirs)
		cmd->redirs = node;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

t_cmd	*create_cmd_node(void)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->in_fd = -1;
	node->out_fd = -1;
	node->args = NULL;
	node->redirs = NULL;
	node->next = NULL;
	node->in_file = NULL;
	node->out_file = NULL;
	node->syntax_error = false;
	node->pid_array = NULL;
	return (node);
}

/**
 * Report a syntax error and set the shell exit status to 2
 */
void	handle_syntax_error(char *token, t_shell *shell)
{
	print_error_message("minishell: syntax error near unexpected token `",
		token, "'\n");
	shell->exit_status = 2;
}

void	cleanup_shell(t_shell *shell)
{
	if (shell && shell->cmd && shell->cmd->pid_array != NULL)
	{
		free(shell->cmd->pid_array);
		shell->cmd->pid_array = NULL;
	}
	if (shell->cmd)
	{
		free_cmd(shell->cmd);
		shell->cmd = NULL;
	}
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	rl_clear_history();
}
