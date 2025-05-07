/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:00:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/07 13:29:39 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_cd_error(t_cmd *cmd, char *old_pwd, char *path_dup)
{
	cd_error(path_dup);
	if (old_pwd != NULL)
		free(old_pwd);
	cmd->shell->exit_status = 1;
}

static char	*get_cd_path(t_cmd *cmd)
{
	char	*path;
	int		i;

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
		return (get_env_value("HOME", cmd->shell->env));
	else if (ft_strcmp(cmd->args[1], "-") == 0)
		return (get_env_value("OLDPWD", cmd->shell->env));
	else
	{
		path = cmd->args[1];
		i = 0;
		if (path && path[0] == '/')
		{
			while (path[i] && path[i] == '/')
				i++;
			if (!path[i])
				return ("/");
		}
		return (path);
	}
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
	if (!old_pwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
		ft_putstr_fd("cannot access parent directories: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
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
	setup_signals();
}
