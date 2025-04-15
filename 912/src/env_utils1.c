/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:15:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/08 17:18:14 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*resolve_path(char *cmd, char **env)
{
	int		i;
	char	*full_path;
	char	*temp;
	char	**paths;

	// Handle NULL or empty command
	if (!cmd || !*cmd)
		return (NULL);
		
	// If command already contains a slash (relative or absolute path)
	// return it directly without searching PATH
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
		
	// If no environment or PATH not set, return command as-is
	if (!env || !get_env_value("PATH", env))
		return (ft_strdup(cmd));
		
	// Split PATH into directories and search in each one
	i = -1;
	paths = ft_split(get_env_value("PATH", env), ':');
	if (!paths)
		return (ft_strdup(cmd));
		
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK) == 0)  // First check if file exists
		{
			if (access(full_path, X_OK) == 0)  // Then check if executable
			{
				free_env(paths);
				return (full_path);
			}
			free(full_path); // File exists but not executable
		}
		else
		{
			free(full_path); // File doesn't exist
		}
	}
	free_env(paths);
	return (ft_strdup(cmd)); // Return command as-is if not found in PATH
}
