/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:58:02 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/03 18:58:02 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_quoted(char *token)
{
	if (!token)
	{
		return (false);
	}
	return ((token[0] == '\'' && token[ft_strlen(token) - 1] == '\'')
		|| (token[0] == '"' && token[ft_strlen(token) - 1] == '"'));
}

bool	is_metacharacter(char *token)
{
	if (is_quoted(token))
		return (false);
	return (ft_strcmp(token, "|") == 0 || ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0
		|| ft_strcmp(token, "<<") == 0);
}

void	add_argument_to_array(char ***args, char *arg)
{
	char	**new_args;
	int		len;
	int		i;

	len = 0;
	while ((*args) && (*args)[len])
		len++;
	new_args = malloc((len + 2) * sizeof(char *));
	if (!new_args)
		return ;
	i = 0;
	while (i < len)
	{
		new_args[i] = (*args)[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
	{
		free(new_args);
		return ;
	}
	new_args[i + 1] = NULL;
	free(*args);
	*args = new_args;
}

void	add_argument(t_cmd *node, char *arg)
{
	if (!node || !arg)
		return ;
	if (!node->args)
	{
		node->args = malloc(2 * sizeof(char *));
		if (!node->args)
			return ;
		node->args[0] = ft_strdup(arg);
		if (!node->args[0])
		{
			safe_free(node->args);
			node->args = NULL;
			return ;
		}
		node->args[1] = NULL;
	}
	else
	{
		add_argument_to_array(&(node->args), arg);
	}
}

void	handle_pipeline(t_cmd **current, t_shell *shell)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd_node();
	if (!new_cmd)
	{
		(*current)->syntax_error = 1;
		return ;
	}
	new_cmd->shell = shell;
	(*current)->next = new_cmd;
	*current = new_cmd;
}
