/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/28 15:40:00 by vagarcia         ###   ########.fr       */
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

static void	handle_quoted(char *arg, int *i, t_expander_state *state)
{
	if (arg[*i] == '\'' && !state->in_dquote)
	{
		state->in_quote = !state->in_quote;
		(*i)++;
	}
	else if (arg[*i] == '"' && !state->in_quote)
	{
		state->in_dquote = !state->in_dquote;
		(*i)++;
	}
}

static void	process_character(char *arg, int *i, t_expander_state *state,
			t_shell *shell)
{
	char	temp[2];

	if (arg[*i] == '\'' && !state->in_dquote)
		handle_quoted(arg, i, state);
	else if (arg[*i] == '"' && !state->in_quote)
		handle_quoted(arg, i, state);
	else if (arg[*i] == '$' && !state->in_quote)
	{
		*i = handle_dollar_sign(arg, *i, state, shell);
		(*i)++;
	}
	else
	{
		temp[0] = arg[*i];
		temp[1] = '\0';
		state->result = append_str(state->result, temp);
		if (!state->result)
			state->result = ft_strdup("");
		(*i)++;
	}
}

char	*process_argument(char *arg, t_shell *shell)
{
	t_expander_state	state;
	int					i;

	ft_memset(&state, 0, sizeof(t_expander_state));
	state.result = ft_strdup("");
	if (!state.result || !arg)
		return (state.result);
	i = 0;
	while (arg[i])
		process_character(arg, &i, &state, shell);
	return (state.result);
}

static void	handle_special_dir(t_cmd *cmd, t_shell *shell, char *error_msg,
			int exit_code)
{
	
	ft_putstr_fd(error_msg, 2);
	shell->exit_status = exit_code;
	free(cmd->args[0]);
	free(cmd->args);
	cmd->args = NULL;
	cmd->syntax_error = 1;
}

void	expander(t_cmd *cmd, t_shell *shell)
{
	char	*expanded;
	
	if (!cmd || !cmd->args)
    	return;
	if (cmd->args[0] && ft_strcmp(cmd->args[0], ".") == 0 && !cmd->args[1])
	{
		handle_special_dir(cmd, shell, "Minishell: .: filename argument required\n.: usage: . filename [arguments]\n",
			2);
		return ;
	}
	if (cmd->args[0] && ft_strcmp(cmd->args[0], "..") == 0 && !cmd->args[1])
	{
		handle_special_dir(cmd, shell, "..: command not found\n", 127);
		return ;
	}
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


void	expand_nodes(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*node;
	char	*dup;
	int		k;

	node = cmd;
	shell->cmd = node;
	while (node)
	{
		expander(node, shell);
		node->shell = shell;
		if (!(node->args && node->args[0] && node->args[0][0] == '\0'))
			resolve_non_builtin_path(node, shell);
		if (node->args && node->args[0] && node->args[0][0] == '\0')
		{
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
		node = node->next;
	}
}
