/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:03:09 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 11:16:34 by vagarcia         ###   ########.fr       */
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
	char		*prompt;
	char		*result;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		return (ft_strdup("Minishell-> "));
	}
	prompt = ft_strjoin(cwd, " -> ");
	if (!prompt)
		return (NULL);
	result = ft_strdup(prompt);
	if (!result)
	{
		free(prompt);
		return (NULL);
	}
	free(prompt);
	return (result);
}

/**
 * Report a syntax error and set the shell exit status to 2
 */
void	handle_syntax_error(char *token, t_shell *shell)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	shell->exit_status = 2;
}
