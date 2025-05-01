/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:00:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 21:23:17 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_cd_path(t_cmd *cmd, char **path, char **path_dup,
		char *old_pwd)
{
	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0
		|| ft_strcmp(cmd->args[1], "-") == 0)
	{
		*path_dup = ft_strdup(*path);
		if (!*path_dup)
		{
			free(old_pwd);
			cmd->shell->exit_status = 1;
			return ;
		}
		*path = *path_dup;
	}
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0 && *path)
		ft_putendl_fd(*path, STDOUT_FILENO);
}

static char	*get_cd_path(t_cmd *cmd)
{
	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
		return (get_env_value("HOME", cmd->shell->env));
	else if (ft_strcmp(cmd->args[1], "-") == 0)
		return (get_env_value("OLDPWD", cmd->shell->env));
	else
		return (cmd->args[1]);
}

static void	handle_cd_path_not_found(t_cmd *cmd, char *old_pwd)
{
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0)
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
	else
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	free(old_pwd);
	cmd->shell->exit_status = 1;
}

static int	check_cd_args(t_cmd *cmd, char *old_pwd)
{
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		free(old_pwd);
		cmd->shell->exit_status = 1;
		return (0);
	}
	return (1);
}

void	ft_cd(t_cmd *cmd)
{
	char	*path;
	char	*old_pwd;
	char	*path_dup;

	old_pwd = getcwd(NULL, 0);
	if (!check_cd_args(cmd, old_pwd))
		return ;
	path = get_cd_path(cmd);
	if (!path)
	{
		handle_cd_path_not_found(cmd, old_pwd);
		return ;
	}
	path_dup = NULL;
	handle_cd_path(cmd, &path, &path_dup, old_pwd);
	if (chdir(path) != 0)
	{
		if (path_dup)
			handle_cd_error(cmd, old_pwd, path_dup);
		else
			handle_cd_error(cmd, old_pwd, path);
		return ;
	}
	finish_cd_success(cmd, old_pwd, path_dup);
}
