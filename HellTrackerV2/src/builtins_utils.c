/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:33:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 20:35:50 by vagarcia         ###   ########.fr       */
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
		/* If getcwd fails (e.g., directory deleted), still update OLDPWD */
		old_pwd = get_env_value("PWD", shell->env);
		if (old_pwd)
		{
			oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
			if (oldpwd_var)
			{
				update_or_add_env(oldpwd_var, &shell->env);
				xfree(oldpwd_var);
			}
		}
		return ;
	}
	old_pwd = get_env_value("PWD", shell->env);
	if (old_pwd)
	{
		oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
		if (oldpwd_var)
		{
			update_or_add_env(oldpwd_var, &shell->env);
			xfree(oldpwd_var);
		}
	}
	new_pwd = ft_strjoin("PWD=", cwd);
	if (new_pwd)
	{
		update_or_add_env(new_pwd, &shell->env);
		xfree(new_pwd);
	}
	xfree(cwd);
}

void	update_shlvl(t_shell *shell)
{
	int		level;
	char	*level_str;
	char	*new_shlvl;

	level = ft_atoi(get_env_value("SHLVL", shell->env));
	if (level < 0)
		level = 0;
	level++;
	level_str = ft_itoa(level);
	if (!level_str)
		return ;
	new_shlvl = ft_strjoin("SHLVL=", level_str);
	xfree(level_str);
	if (new_shlvl)
	{
		update_or_add_env(new_shlvl, &shell->env);
		xfree(new_shlvl);
	}
}
