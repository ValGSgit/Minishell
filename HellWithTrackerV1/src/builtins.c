/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:33:20 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 20:42:56 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_cd(t_cmd *cmd)
{
	char	*home_dir;
	//char	*current_dir;

	if (!cmd->args[1])
	{
		home_dir = get_env_value("HOME", cmd->shell->env);
		if (!home_dir)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			cmd->shell->exit_status = 1;
			return ;
		}
		if (chdir(home_dir) != 0)
		{
			perror("minishell: cd");
			cmd->shell->exit_status = 1;
			return ;
		}
	}
	else if (chdir(cmd->args[1]) != 0)
	{
		perror("minishell: cd");
		cmd->shell->exit_status = 1;
		return ;
	}
	update_pwd(cmd->shell);
	cmd->shell->exit_status = 0;
}

void	ft_pwd(t_cmd *cmd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		free(cwd);
		cmd->shell->exit_status = 0;
	}
	else
	{
		perror("minishell: pwd");
		cmd->shell->exit_status = 1;
	}
}

void	ft_env(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->args[1])
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		cmd->shell->exit_status = 1;
		return ;
	}
	if (!cmd->shell->env)
	{
		cmd->shell->exit_status = 0;
		return ;
	}
	while (cmd->shell->env[i])
	{
		if (ft_strchr(cmd->shell->env[i], '='))
		{
			ft_putstr_fd(cmd->shell->env[i], 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
	cmd->shell->exit_status = 0;
}
