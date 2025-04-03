/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:15:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/03 14:39:12 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*resolve_path(char *cmd, char **env)
{
	int		i;
	char	*full_path;
	char	*temp;
	char	**paths;

	if (!cmd || !env)
		return (NULL);
	i = -1;
	paths = ft_split(get_env_value("PATH", env), ':');
	if (!paths)
		return (ft_strdup(cmd));
	while (paths[++i])
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
	}
	free_env(paths);
	return (ft_strdup(cmd));
}

static void	replace_env_value(char *key, char *value, char **env_entry)
{
	char	*temp;
	char	*new_value;

	temp = ft_strjoin(key, "=");
	if (!temp)
		return ;
	new_value = ft_strjoin(temp, value);
	free(temp);
	if (!new_value)
		return ;
	free(*env_entry);
	*env_entry = new_value;
}

void	add_new_env_entry(char *key, char **env)
{
	char	*temp;

	temp = ft_strjoin(key, "=");
	if (!temp)
		return ;
	add_new_env_var(temp, env);
	free(temp);
}

void	update_env_value(char *key, char *value, char **env)
{
	int	i;

	if (!key || !value || !env)
		return ;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0
			&& env[i][ft_strlen(key)] == '=')
		{
			replace_env_value(key, value, &env[i]);
			return ;
		}
		i++;
	}
	add_new_env_entry(key, env);
}

void	update_or_add_env(char *arg, char **env)
{
	char	*key;
	char	*value;
	char	*eq;

	if (!arg || !env)
		return ;
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, eq - arg);
		value = ft_strdup(eq + 1);
		if (value)
		{
			update_env_value(key, value, env);
			free(key);
			free(value);
		}
	}
	else
		add_new_env_var(arg, env);
}
