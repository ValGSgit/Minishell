/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:00:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 20:15:49 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Creates and prints a single error message to stderr
 * Takes variable parts and joins them before printing
 */
void	print_error_message(const char *prefix, const char *msg, const char *extra)
{
	char	*error_msg;
	char	*temp;

	if (!prefix && !msg && !extra)
		return;
	if (prefix)
		error_msg = ft_strdup(prefix);
	else
		error_msg = ft_strdup("");
	if (!error_msg)
		return;
	if (msg)
	{
		temp = error_msg;
		error_msg = ft_strjoin(error_msg, msg);
		free(temp);
		if (!error_msg)
			return;
	}
	if (extra)
	{
		temp = error_msg;
		error_msg = ft_strjoin(error_msg, extra);
		free(temp);
		if (!error_msg)
			return;
	}
	ft_putstr_fd(error_msg, 2);
	free(error_msg);
}

static void	cd_error(char *path_dup)
{
	char *error_msg;
	
	error_msg = ft_strjoin("minishell: cd: ", path_dup);
	if (!error_msg)
		return;
	
	char *temp = error_msg;
	char *with_colon = ft_strjoin(error_msg, ": ");
	free(temp);
	if (!with_colon)
		return;
	
	temp = with_colon;
	error_msg = ft_strjoin(with_colon, strerror(errno));
	free(temp);
	if (!error_msg)
		return;
	
	temp = error_msg;
	error_msg = ft_strjoin(error_msg, "\n");
	free(temp);
	
	if (error_msg)
	{
		ft_putstr_fd(error_msg, 2);
		free(error_msg);
	}
}

static void	handle_cd_error(t_cmd *cmd, char *old_pwd, char *path_dup)
{
	cd_error(path_dup);
	if (old_pwd != NULL)
		free(old_pwd);
	cmd->shell->exit_status = 1;
}

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

void	ft_cd(t_cmd *cmd)
{
	char	*path;
	char	*old_pwd;
	char	*path_dup;

	old_pwd = getcwd(NULL, 0);
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		free(old_pwd);
		cmd->shell->exit_status = 1;
		return ;
	}
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
	update_pwd(cmd->shell);
	free(old_pwd);
	if (path_dup)
		free(path_dup);
	cmd->shell->exit_status = 0;
}
