/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_things.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:15:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/01 13:46:38 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**copy_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			free_env(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

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


void	update_env_value(char *key, char *value, char **env)
{
	int	i;

	i = 0;
	if (!key || !value || !env)
		return;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0 && env[i][ft_strlen(key)] == '=')
		{
			free(env[i]);
			env[i] = ft_strjoin(key, "=");
			env[i] = ft_strjoin(env[i], value);
			return;
		}
		i++;
	}
	add_new_env_var(ft_strjoin(key, "="), env);
}

void	update_or_add_env(char *arg, char **env)
{
	char	*key;
	char	*value;
	char	*eq;

	if (!arg || !env)
		return;
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, eq - arg);
		value = ft_strdup(eq + 1);
		update_env_value(key, value, env);
		free(key);
		free(value);
	}
	else
		add_new_env_var(arg, env);
}
