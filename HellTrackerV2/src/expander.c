/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 22:30:12 by vagarcia         ###   ########.fr       */
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
	xfree(dest);
	return (new_str);
}

static char	*process_single_quoted(char *arg, int *i, t_expander_state *state)
{
	int		j;
	char	*value;

	state->in_quote = 1;
	(*i)++;
	j = *i;
	while (arg[*i] && arg[*i] != '\'')
		(*i)++;
	value = ft_substr(arg, j, *i - j);
	if (arg[*i] == '\'')
		(*i)++;  /* Skip closing quote */
	state->in_quote = 0;
	return (value);
}

static char	*process_double_quoted(char *arg, int *i, t_expander_state *state, 
								t_shell *shell)
{
	int		j;
	char	*result;
	char	*value;

	state->in_dquote = 1;
	(*i)++;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (arg[*i] && arg[*i] != '"')
	{
		if (arg[*i] == '$')
		{
			j = *i;
			value = expand_variable(arg, i, shell);
			if (value)
				result = append_str(result, value);
			xfree(value);
		}
		else
		{
			result = append_str(result, (char[]){arg[*i], '\0'});
			(*i)++;
		}
	}
	if (arg[*i] == '"')
		(*i)++;  /* Skip closing quote */
	state->in_dquote = 0;
	return (result);
}

static char	*process_dollar_quoted(char *arg, int *i)
{
	int		j;
	char	*value;

	(*i) += 2;  /* Skip $" */
	j = *i;
	/* Find closing quote */
	while (arg[*i] && arg[*i] != '"')
		(*i)++;
	value = ft_substr(arg, j, *i - j);
	if (arg[*i] == '"')
		(*i)++;  /* Skip closing quote */
	return (value);
}

static char	*process_dollar_token(char *arg, int *i, t_shell *shell)
{
	char	*value;

	value = expand_variable(arg, i, shell);
	return (value);
}

char	*process_argument(char *arg, t_shell *shell)
{
	t_expander_state	state;
	int					i;
	char				*value;

	ft_memset(&state, 0, sizeof(t_expander_state));
	state.result = ft_strdup("");
	if (!state.result || !arg)
		return (state.result);
	i = 0;
	while (arg[i])
	{
		/* Handle single quotes - preserve content literally */
		if (arg[i] == '\'' && !state.in_dquote)
		{
			value = process_single_quoted(arg, &i, &state);
			state.result = append_str(state.result, value);
			xfree(value);
		}
		/* Handle double quotes - expand variables inside */
		else if (arg[i] == '"' && !state.in_quote)
		{
			value = process_double_quoted(arg, &i, &state, shell);
			state.result = append_str(state.result, value);
			xfree(value);
		}
		/* Handle $"string" - preserve string literally without expanding */
		else if (arg[i] == '$' && i + 1 < (int)ft_strlen(arg) && arg[i + 1] == '"')
		{
			value = process_dollar_quoted(arg, &i);
			state.result = append_str(state.result, value);
			xfree(value);
		}
		/* Handle $ outside of quotes - expand variables */
		else if (arg[i] == '$' && !state.in_quote)
		{
			value = process_dollar_token(arg, &i, shell);
			if (value)
				state.result = append_str(state.result, value);
			xfree(value);
		}
		/* Handle regular characters */
		else
		{
			state.result = append_str(state.result, (char[]){arg[i], '\0'});
			i++;
		}
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
		return (NULL);
	path = ft_strjoin(current_dir, "/");
	xfree(current_dir);
	if (!path)
		return (NULL);
	temp = path;
	path = ft_strjoin(path, cmd);
	xfree(temp);
	xfree(cmd);
	return (path);
}

static void	handle_dot_cmds(t_cmd *cmd, t_shell *shell)
{
	if (cmd->args[0] && ft_strcmp(cmd->args[0], ".") == 0 && !cmd->args[1])
	{
		ft_putstr_fd("Minishell: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		shell->exit_status = 2;
		cmd->args = NULL;
		cmd->syntax_error = 1;
		return ;
	}
	if (cmd->args[0] && ft_strcmp(cmd->args[0], "..") == 0 && !cmd->args[1])
	{
		ft_putstr_fd("..: command not found\n", 2);
		shell->exit_status = 127;
		cmd->args = NULL;
		cmd->syntax_error = 1;
	}
}

static void	resolve_cmd_path(t_cmd *cmd, t_shell *shell)
{
	char	*expanded;

	if (cmd->args[0] && ft_strchr(cmd->args[0], '/') == NULL
		&& !is_builtin(cmd->args[0]))
	{
		if (shell->env && get_env_value("PATH", shell->env))
			expanded = resolve_path(cmd->args[0], shell->env);
		else
			expanded = resolve_path_in_current_dir(cmd->args[0]);
		cmd->args[0] = expanded;
	}
}

char	*resolve_path(char *cmd, char **env)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (!cmd || !*cmd || !env || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value("PATH", env);
	if (!path_env)
		return (ft_strdup(cmd));
	paths = ft_split(path_env, ':');
	if (!paths)
		return (ft_strdup(cmd));
	i = -1;
	full_path = NULL;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			continue;
		full_path = ft_strjoin(temp, cmd);
		xfree(temp);
		if (full_path && access(full_path, F_OK | X_OK) == 0)
			break;
		xfree(full_path);
		full_path = NULL;
	}
	ft_free(paths);
	return (full_path ? full_path : ft_strdup(cmd));
}

void	expander(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->args)
		return ;
	handle_dot_cmds(cmd, shell);
	if (cmd->syntax_error)
		return ;
	resolve_cmd_path(cmd, shell);
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
				reresolved = resolve_path_in_current_dir(node->args[0]);
			if (node->args[0])
				xfree(node->args[0]);
			node->args[0] = reresolved;
		}
		node = node->next;
	}
}
