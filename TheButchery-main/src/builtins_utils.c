/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:33:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 12:36:26 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_pwd(t_shell *shell)
{
	char	*cwd;
	char	*new_pwd;
	char	*old_pwd;
	char	*oldpwd_var;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		// If getcwd fails (e.g., directory deleted), still update OLDPWD
		old_pwd = get_env_value("PWD", shell->env);
		if (old_pwd)
		{
			oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
			if (oldpwd_var)
			{
				update_or_add_env(oldpwd_var, &shell->env);
				free(oldpwd_var);
			}
		}
		return;
	}

	old_pwd = get_env_value("PWD", shell->env);
	if (old_pwd)
	{
		oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
		if (oldpwd_var)
		{
			update_or_add_env(oldpwd_var, &shell->env);
			free(oldpwd_var);
		}
	}
	
	new_pwd = ft_strjoin("PWD=", cwd);
	if (new_pwd)
	{
		update_or_add_env(new_pwd, &shell->env);
		free(new_pwd);
	}
	free(cwd);
}

void	update_shlvl(t_shell *shell)
{
	//char	*shlvl_value;
	int		level;
	char	*level_str;
	char	*new_shlvl;

	//shlvl_value = get_env_value("SHLVL", shell->env);
	level = ft_atoi(get_env_value("SHLVL", shell->env));
	if (level < 0)
		level = 0;
	level++;
	level_str = ft_itoa(level);
	if (!level_str)
		return ;
	new_shlvl = ft_strjoin("SHLVL=", level_str);
	free(level_str);
	if (new_shlvl)
	{
		update_or_add_env(new_shlvl, &shell->env);
		free(new_shlvl);
	}
}
void	ft_cd(t_cmd *cmd)
{
	char	*path;
	char	*old_pwd;
	char	*path_dup;

	// Save current directory for updating OLDPWD
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cmd->shell->exit_status = 1;
		return ;
	}
	// Get target path
	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
		path = get_env_value("HOME", cmd->shell->env);
	else if (ft_strcmp(cmd->args[1], "-") == 0)
		path = get_env_value("OLDPWD", cmd->shell->env);
	else
		path = cmd->args[1];
	if (!path)
	{
		if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0)
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		else
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		free(old_pwd);
		cmd->shell->exit_status = 1;
		return ;
	}
	
	// Make a copy of path if it's from environment (HOME or OLDPWD)
	path_dup = NULL;
	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0 || 
		ft_strcmp(cmd->args[1], "-") == 0)
	{
		path_dup = ft_strdup(path);
		if (!path_dup)
		{
			free(old_pwd);
			cmd->shell->exit_status = 1;
			return;
		}
		path = path_dup;
	}

	// Print path when using cd -
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0 && path)
		ft_putendl_fd(path, STDOUT_FILENO);
	
	// Change directory
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		free(old_pwd);
		free(path_dup);
		cmd->shell->exit_status = 1;
		return ;
	}
	
	update_pwd(cmd->shell);
	free(old_pwd);
	free(path_dup);
	cmd->shell->exit_status = 0;
}

void	ft_pwd(t_cmd *cmd)
{
	char	cwd[PATH_MAX];

	// pwd should ignore arguments
	if (getcwd(cwd, 1024) != NULL)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		cmd->shell->exit_status = 0;
	}
	else
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cmd->shell->exit_status = 1;
	}
}

void	ft_env(t_cmd *cmd)
{
	int	i;

	// No environment is an error
	if (!cmd->shell->env)
	{
		cmd->shell->exit_status = 1;
		return;
	}
	
	// env command doesn't accept arguments in basic shell implementation
	if (cmd->args[1])
	{
		ft_putstr_fd("minishell: env: '", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd("': No such file or directory\n", 2);
		cmd->shell->exit_status = 127;
		return;
	}
	
	// Print environment variables
	i = 0;
	while (cmd->shell->env[i])
	{
		// Only print variables with values (containing an equals sign)
		if (ft_strchr(cmd->shell->env[i], '='))
			ft_putendl_fd(cmd->shell->env[i], STDOUT_FILENO);
		i++;
	}
	
	cmd->shell->exit_status = 0;
}
