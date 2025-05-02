/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:15:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/28 14:40:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*try_path(char *path, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK) == 0)
	{
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	else
		free(full_path);
	return (NULL);
}

static char	*check_direct_path(char *cmd)
{
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*try_paths(char **paths, char *cmd)
{
	int		i;
	char	*result;

	i = 0;
	while (paths[i])
	{
		result = try_path(paths[i], cmd);
		if (result)
			return (result);
		i++;
	}
	return (NULL);
}

char	*resolve_path(char *cmd, char **env)
{
	char	*direct_path;
	char	*result;
	char	**paths;

	direct_path = check_direct_path(cmd);
	if (direct_path)
		return (direct_path);
	if (!env || !get_env_value("PATH", env))
		return (ft_strdup(cmd));
	paths = ft_split(get_env_value("PATH", env), ':');
	if (!paths)
		return (ft_strdup(cmd));
	result = try_paths(paths, cmd);
	free_env(paths);
	if (result)
		return (result);
	return (ft_strdup(cmd));
}

/* Checks if the token contains a metacharacter */
bool	contains_metacharacter(char *token)
{
	if (!token)
		return (false);
	return (ft_strchr(token, '|') || ft_strchr(token, '<') || ft_strchr(token,
			'>'));
}
