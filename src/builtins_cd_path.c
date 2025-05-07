/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:43:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/07 13:34:29 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_all_slashes(char *path)
{
	int	i;

	i = 0;
	while (path[i] && path[i] == '/')
		i++;
	if (!path[i])
		return (1);
	return (0);
}

char	*get_cd_path(t_cmd *cmd)
{
	char	*path;

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
		return (get_env_value("HOME", cmd->shell->env));
	else if (ft_strcmp(cmd->args[1], "-") == 0)
		return (get_env_value("OLDPWD", cmd->shell->env));
	else
	{
		path = cmd->args[1];
		if (path && path[0] == '/' && is_all_slashes(path))
			return ("/");
		return (path);
	}
}

void	finish_cd_success(t_cmd *cmd, char *old_pwd, char *path_dup)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (path_dup)
			update_logical_pwd(cmd->shell, path_dup);
		else if (cmd->args[1])
			update_logical_pwd(cmd->shell, cmd->args[1]);
	}
	else
	{
		update_pwd(cmd->shell);
		free(cwd);
	}
	free(old_pwd);
	if (path_dup)
		free(path_dup);
	cmd->shell->exit_status = 0;
}
