/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:03:09 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/02 15:09:22 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	create_redir_node(t_cmd *cmd, int type, char *file)
{
	t_redir	*node;
	t_redir	*tmp;

	node = malloc(sizeof(t_redir));
	if (!node)
		return ;
	node->type = type;
	node->file = process_argument(file, cmd->shell);
	node->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = node;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
	return ;
}

t_cmd	*create_cmd_node(void)
{
	t_cmd	*node;

	node = ft_calloc(sizeof(t_cmd), 1);
	if (!node)
		return (NULL);
	node->in_fd = STDIN_FILENO;
	node->out_fd = STDOUT_FILENO;
	node->args = NULL;
	node->redirs = NULL;
	node->next = NULL;
	return (node);
}

/* Updates the shell prompt based on the current working directory */
char	*update_prompt(void)
{
	static char	cwd[1024];
	char	*prompt;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		return (ft_strdup("Minishell-> "));
	}
	prompt = ft_strjoin(cwd, " -> ");
	if (!prompt)
		return (NULL);
	return (prompt);
}
