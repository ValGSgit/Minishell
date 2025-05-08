/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:33:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 20:47:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_oldpwd(t_shell *shell, char *old_pwd)
{
	char	*oldpwd_var;

	if (old_pwd)
	{
		oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
		if (oldpwd_var)
		{
			update_or_add_env(oldpwd_var, &shell->env);
			free(oldpwd_var);
		}
	}
}

void	update_pwd(t_shell *shell)
{
	char	*cwd;
	char	*new_pwd;
	char	*old_pwd;

	cwd = getcwd(NULL, 0);
	if (shell->env)
		old_pwd = get_env_value("PWD", shell->env);
	else
		old_pwd = NULL;
	update_oldpwd(shell, old_pwd);
	if (!cwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory: getcwd: ", 2);
		ft_putstr_fd("cannot access parent directories: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return ;
	}
	new_pwd = ft_strjoin("PWD=", cwd);
	if (new_pwd)
	{
		update_or_add_env(new_pwd, &shell->env);
		free(new_pwd);
	}
	free(cwd);
}

static void	set_default_shlvl(t_shell *shell)
{
	char	*new_shlvl;

	new_shlvl = ft_strdup("SHLVL=1");
	if (new_shlvl)
	{
		update_or_add_env(new_shlvl, &shell->env);
		free(new_shlvl);
	}
}

void	update_shlvl(t_shell *shell)
{
	int		level;
	char	*level_str;
	char	*new_shlvl;
	char	*val;

	val = get_env_value("SHLVL", shell->env);
	if (!val)
	{
		set_default_shlvl(shell);
		return ;
	}
	level = ft_atoi(val);
	if (level < 0)
		level = 0;
	level_str = ft_itoa(++level);
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

void	ft_pwd(t_cmd *cmd)
{
	char	cwd[1024];
	char	*pwd_env;

	if (getcwd(cwd, 1024) != NULL)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		cmd->shell->exit_status = 0;
	}
	else
	{
		pwd_env = get_env_value("PWD", cmd->shell->env);
		if (pwd_env)
		{
			ft_putendl_fd(pwd_env, STDOUT_FILENO);
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
}
