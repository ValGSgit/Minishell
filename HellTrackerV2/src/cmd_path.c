/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:41:12 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 17:41:12 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*construct_cmd_path(char *dir, char *cmd)
{
	char	*path;
	char	*tmp;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

static char	*find_cmd_path(char *cmd, char **env_paths)
{
	int		i;
	char	*cmd_path;

	i = 0;
	while (env_paths && env_paths[i])
	{
		cmd_path = construct_cmd_path(env_paths[i], cmd);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, t_shell *shell)
{
	char	**env_paths;
	char	*path_env;
	char	*cmd_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	if (shell->path_unset)
		return (NULL);
	path_env = get_env_value("PATH", shell->env);
	if (!path_env)
		return (NULL);
	env_paths = ft_split(path_env, ':');
	if (!env_paths)
		return (NULL);
	cmd_path = find_cmd_path(cmd, env_paths);
	free_tokens(env_paths);
	return (cmd_path);
}