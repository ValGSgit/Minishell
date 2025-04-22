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

char	*process_argument(char *arg, t_shell *shell)
{
	t_expander_state	state;
	int					i;
	char				*value;

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
		{
			value = expand_variable(arg, &i, shell);
			if ((value && *value))
				state.result = append_str(state.result, value);
			free(value);
		}
		else
			state.result = append_str(state.result, (char[]){arg[i], '\0'});
	}
	return (state.result);
}

static char	*resolve_path_in_current_dir(char *cmd)
{
	char	*path;
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (NULL);
	path = ft_strjoin(current_dir, "/");
	free(current_dir);
	if (!path)
		return (NULL);
	path = ft_strjoin(path, cmd);
	free(cmd);
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
				free(node->args[0]);
			node->args[0] = reresolved;
		}
		node = node->next;
	}
}
