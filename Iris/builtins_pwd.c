/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iionescu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:58:31 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/01 15:58:32 by iionescu         ###   ########.fr       */
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
	if (!cwd)
	{
		old_pwd = get_env_value("PWD", shell->env);
		update_oldpwd(shell, old_pwd);
		return ;
	}
	old_pwd = get_env_value("PWD", shell->env);
	update_oldpwd(shell, old_pwd);
	new_pwd = ft_strjoin("PWD=", cwd);
	if (new_pwd)
	{
		update_or_add_env(new_pwd, &shell->env);
		free(new_pwd);
	}
	free(cwd);
}

void	ft_pwd(t_cmd *cmd)
{
	char	cwd[1024];

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