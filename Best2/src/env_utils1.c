/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:15:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/26 21:10:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Resolves a command to its full path using PATH environment variable
 * Returns a newly allocated string with the full path, or a copy of cmd
 * if the command is not found in PATH
 */
char	*resolve_path(char *cmd, char **env)
{
	int		i;
	char	*full_path;
	char	*temp;
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	if (!env || !get_env_value("PATH", env))
		return (ft_strdup(cmd));
	i = -1;
	paths = ft_split(get_env_value("PATH", env), ':');
	if (!paths)
		return (ft_strdup(cmd));
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			continue ;
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			continue ;
		if (access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) == 0)
			{
				free_env(paths);
				return (full_path);
			}
			free(full_path);
		}
		else
			free(full_path);
	}
	free_env(paths);
	return (ft_strdup(cmd));
}
