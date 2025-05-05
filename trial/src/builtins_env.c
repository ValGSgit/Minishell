/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:42:34 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/28 13:42:57 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* static void	process_env_args(t_cmd *cmd, int k)
{
	char	*tmp;

	tmp = ft_strdup(cmd->args[k]);
	free(cmd->args[k - 1]);
	free(cmd->args[k]);
	cmd->args[k - 1] = tmp;
	cmd->args[k] = NULL;
} */

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
	//int	k;

/* 	k = 1;
	while (cmd->args[k])
	{
		process_env_args(cmd, k);
		k++;
		if (!cmd->args[k])
		{
			executor(cmd, cmd->shell);
			return ;
		}
	} */
	if (cmd->args[1])
	{
		
	}
	if (!cmd->shell->env)
	{
		cmd->shell->exit_status = 1;
		return ;
	}
	display_env_vars(cmd);
}
