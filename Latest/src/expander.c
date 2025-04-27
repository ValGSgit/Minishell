/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/26 20:45:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*append_str(char *dest, char *src)
{
	char	*new_str;

	if (!dest)
	{
		if (src)
			return (ft_strdup(src));
		else
			return (ft_strdup(""));
	}
	if (!src)
		src = "";
	new_str = ft_strjoin(dest, src);
	if (!new_str)
		return (dest);
	free(dest);
	return (new_str);
}

static int	handle_dollar_sign(char *arg, int i, t_expander_state *state,
			t_shell *shell)
{
	char	*value;

	// Special case: trailing $ at the end of string
	if (!arg[i + 1])
	{
		state->result = append_str(state->result, "$");
		return (i);
	}
	// Handle normal expansion if not in single quotes
	if (!state->in_quote)
	{
		value = expand_variable(arg, &i, shell);
		if (value)
		{
			state->result = append_str(state->result, value);
			free(value);
		}
		return (i);
	}
	// Just append literal $ if in single quotes
	state->result = append_str(state->result, "$");
	return (i);
}

/**
 * Processes a single argument, expanding any variables as needed
 * Returns the expanded string
 */
char	*process_argument(char *arg, t_shell *shell)
{
	t_expander_state	state;
	int					i;

	ft_memset(&state, 0, sizeof(t_expander_state));
	state.result = ft_strdup("");
	if (!state.result || !arg)
		return (state.result);
	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '\'' && !state.in_dquote)
			state.in_quote = !state.in_quote;
		else if (arg[i] == '"' && !state.in_quote)
			state.in_dquote = !state.in_dquote;
		else if (arg[i] == '$' && !state.in_quote)
			i = handle_dollar_sign(arg, i, &state, shell);
		else
			state.result = append_str(state.result, (char[]){arg[i], '\0'});
	}
	return (state.result);
}

static char	*resolve_path_in_current_dir(char *cmd)
{
	char	*path;
	char	*current_dir;
	char	*temp;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (ft_strdup(cmd));
	path = ft_strjoin(current_dir, "/");
	free(current_dir);
	if (!path)
		return (ft_strdup(cmd));
	temp = path;
	path = ft_strjoin(path, cmd);
	free(temp);
	return (path);
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
		return ;
	if (cmd->args[0] && ft_strcmp(cmd->args[0], ".") == 0 && !cmd->args[1])
	{
		handle_special_dir(cmd, shell, 
			"Minishell: .: filename argument required\n.: usage: . filename [arguments]\n",
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
			expanded = resolve_path_in_current_dir(ft_strdup(cmd->args[0]));
		free(cmd->args[0]);
		cmd->args[0] = expanded;
	}
	cmd->args = apply_word_splitting(cmd->args, shell);
}

void	expand_nodes(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*node;
	char	*reresolved;

	node = cmd;
	shell->cmd = node;
	while (node)
	{
		expander(node, shell);
		node->shell = shell;
		if (node->args && node->args[0] && ft_strchr(node->args[0], '/') == NULL
			&& !is_builtin(node->args[0]))
		{
			if (shell->env && get_env_value("PATH", shell->env))
				reresolved = resolve_path(node->args[0], shell->env);
			else
				reresolved = resolve_path_in_current_dir(ft_strdup(node->args[0]));
			free(node->args[0]);
			node->args[0] = reresolved;
		}
		node = node->next;
	}
}
