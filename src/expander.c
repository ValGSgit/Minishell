/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/01 12:09:41 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(char *name, char **env)
{
	int		i;
	char	*key;

	i = 0;
	if (!env || !name)
		return (NULL);
	while (env[i])
	{
		key = ft_substr(env[i], 0, (char *)ft_strchr(env[i], '=') - env[i]);
		if (ft_strncmp(key, name, ft_strlen(key)) == 0)
		{
			free(key);
			return (ft_strchr(env[i], '=') + 1);
		}
		free(key);
		i++;
	}
	return (NULL);
}

char	*resolve_path(char *cmd, char **env)
{
	int		i;
	char	*full_path;
	char	*temp;
	char	**paths;

	i = 0;
	paths = ft_split(get_env_value("PATH", env), ':');
	if (!paths)
		return (ft_strdup(cmd));
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free_env(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_env(paths);
	return (ft_strdup(cmd));
}

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
	if (src)
		new_str = ft_strjoin(dest, src);
	else
		new_str = ft_strjoin(dest, "");
	free(dest);
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
			value = expand_variable(arg, &i, shell, state.in_dquote);
			state.result = append_str(state.result, value);
			free(value);
		}
		else
			state.result = append_str(state.result, (char[]){arg[i], 0});
	}
	return (state.result);
}

void	expander(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*expanded;

	if (!cmd || !cmd->args)
		return ;
	if ((ft_strncmp(cmd->args[0], "./", 2) != 0 || ft_strncmp(cmd->args[0],
				"../", 3)) && !is_builtin(cmd->args[0]))
	{
		expanded = resolve_path(cmd->args[0], shell->env);
		free(cmd->args[0]);
		cmd->args[0] = expanded;
	}
	i = 0;
	while (cmd->args[i])
	{
		expanded = process_argument(cmd->args[i], shell);
		if (expanded)
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		i++;
	}
}

/* Remove null or empty strings from args array */
static char	**ft_clean_args(char **args)
{
	int		i;
	int		j;
	char	**cleaned;

	i = 0;
	j = 0;
	if (!args)
		return (NULL);
	while (args[i])
		i++;
	cleaned = malloc(sizeof(char *) * (i + 1));
	i = 0;
	if (!cleaned)
		return (NULL);
	while (args[i])
	{
		if (args[i] && *args[i])
			cleaned[j++] = args[i];
		else
			free(args[i]);
		i++;
	}
	cleaned[j] = NULL;
	free(args);
	return (cleaned);
}

void	expand_nodes(t_cmd *cmd, t_shell *shell)
{
	t_cmd *node;

	node = cmd;
	while (node)
	{
		expander(node, shell);
		node->shell = shell;
		if (node->args)
			node->args = ft_clean_args(node->args);
		node->env = copy_env(shell->env);
		node = node->next;
	}
}
