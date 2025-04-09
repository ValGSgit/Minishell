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
		return ;
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
	char	*new_pwd;
	char	cwd[1024];
	char	*old_pwd_var;

	// Save current directory for updating OLDPWD
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("cd: getcwd");
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
		ft_putstr_fd("cd: directory not set\n", STDERR_FILENO);
		free(old_pwd);
		cmd->shell->exit_status = 1;
		return ;
	}
	// Change directory
	if (chdir(path) != 0)
	{
		perror("cd");
		free(old_pwd);
		cmd->shell->exit_status = 1;
		return ;
	}
	update_pwd(cmd->shell);
	// Update PWD environment variable
	if (getcwd(cwd, 1024))
	{
		new_pwd = ft_strjoin("PWD=", cwd);
		update_or_add_env(new_pwd, &cmd->shell->env);
		free(new_pwd);
	}
	// Update OLDPWD environment variable
	if (old_pwd)
	{
		old_pwd_var = ft_strjoin("OLDPWD=", old_pwd);
		update_or_add_env(old_pwd_var, &cmd->shell->env);
		free(old_pwd_var);
	}
	free(old_pwd);
	cmd->shell->exit_status = 0;
}

void	ft_pwd(t_cmd *cmd)
{
	char	cwd[1024];

	// if (cmd->args[1])
	// {
	// 	write(2, "too many arguments\n", 20);
	// 	cmd->exit_status = 1;
	// 	//return ;
	// }
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
	cmd->shell->exit_status = 0;
}

void	ft_env(t_cmd *cmd)
{
	int	i;

	if (!cmd->shell->env)
		return ;
	if (cmd->args[1])
	{
		cmd->shell->exit_status = 1;
		return ;
	}
	i = 0;
	while (cmd->shell->env[i])
		printf("%s\n", cmd->shell->env[i++]);
}
