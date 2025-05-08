/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:57:37 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/05 13:27:48 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_dollar_sign(char *arg, int i, t_expander_state *state,
	t_shell *shell)
{
	char	*value;

	if (!arg[i + 1])
	{
		state->result = append_str(state->result, "$");
		if (!state->result)
			state->result = ft_strdup("$");
		return (i);
	}
	if (!state->in_quote)
	{
		value = expand_variable(arg, &i, shell);
		if (value)
		{
			state->result = append_str(state->result, value);
			if (!state->result)
				state->result = ft_strdup(value);
			free(value);
		}
		return (i);
	}
	state->result = append_str(state->result, "$");
	if (!state->result)
		state->result = ft_strdup("$");
	return (i);
}

static int	handle_dot_cases(t_cmd *cmd, t_shell *shell)
{
	if (cmd->args[0] && !cmd->args[1] && (ft_strcmp(cmd->args[0], ".") == 0
			|| ft_strcmp(cmd->args[0], "'.'") == 0
			|| ft_strcmp(cmd->args[0], "\".\"") == 0))
	{
		handle_special_dir(cmd, shell,
			"Minishell: .: filename argument required\n.: \
			usage: . filename [arguments]\n",
			2);
		return (0);
	}
	else if (cmd->args[0] && !cmd->args[1]
		&& (ft_strcmp(cmd->args[0], "..") == 0
			|| ft_strcmp(cmd->args[0], "'..'") == 0
			|| ft_strcmp(cmd->args[0], "\"..\"") == 0))
	{
		handle_special_dir(cmd, shell, "..: command not found\n", 127);
		return (0);
	}
	return (1);
}

void	expander(t_cmd *cmd, t_shell *shell)
{
	char	*expanded;

	if (!cmd || !cmd->args)
		return ;
	if (!handle_dot_cases(cmd, shell))
		return ;
	if (cmd->args[0] && ft_strchr(cmd->args[0], '/') == NULL
		&& !is_builtin(cmd->args[0]))
	{
		if (shell->env && get_env_value("PATH", shell->env))
			expanded = resolve_path(cmd->args[0], shell->env);
		else
			expanded = resolve_path_in_current_dir(cmd->args[0]);
		free(cmd->args[0]);
		cmd->args[0] = expanded;
	}
	cmd->args = apply_word_splitting(cmd->args, shell);
}

static void	handle_empty_argument(t_cmd *node)
{
	int		k;
	char	*dup;

	k = 1;
	while (node->args[k])
	{
		dup = ft_strdup(node->args[k]);
		safe_free(node->args[k - 1]);
		safe_free(node->args[k]);
		node->args[k - 1] = dup;
		node->args[k] = NULL;
		k++;
	}
}

void	expand_nodes(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*node;

	node = cmd;
	shell->cmd = node;
	while (node)
	{
		expander(node, shell);
		node->shell = shell;
		if (!(node->args && node->args[0] && node->args[0][0] == '\0'))
			resolve_non_builtin_path(node, shell);
		if (node->args && node->args[0] && node->args[0][0] == '\0')
			handle_empty_argument(node);
		node = node->next;
	}
}
