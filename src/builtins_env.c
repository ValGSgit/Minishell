/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:42:34 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 16:32:02 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	display_env_vars(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->shell->env[i])
	{
		if (ft_strchr(cmd->shell->env[i], '='))
			ft_putendl_fd(cmd->shell->env[i], STDOUT_FILENO);
		i++;
	}
	cmd->shell->exit_status = 0;
}

void	ft_env(t_cmd *cmd)
{
	if (cmd->args[1])
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		cmd->shell->exit_status = 1;
		return ;
	}
	if (!cmd->shell->env)
	{
		cmd->shell->exit_status = 1;
		return ;
	}
	display_env_vars(cmd);
}
