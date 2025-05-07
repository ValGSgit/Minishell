/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:20:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/05 10:48:03 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_env_array(char **env, int j)
{
	while (env[j + 1])
	{
		env[j] = env[j + 1];
		j++;
	}
	env[j] = NULL;
}

static void	remove_env_var(t_cmd *cmd, char *arg)
{
	int		i;
	int		j;
	size_t	arg_len;
	bool	is_path;

	i = 0;
	arg_len = ft_strlen(arg);
	is_path = (ft_strcmp(arg, "PATH") == 0);
	if (!cmd->shell->env)
		return ;
	while (cmd->shell->env[i])
	{
		if ((ft_strncmp(cmd->shell->env[i], arg, arg_len) == 0)
			&& (cmd->shell->env[i][arg_len] == '='
			|| cmd->shell->env[i][arg_len] == '\0'))
		{
			free(cmd->shell->env[i]);
			j = i;
			update_env_array(cmd->shell->env, j);
			if (is_path)
				cmd->shell->path_unset = true;
			return ;
		}
		i++;
	}
}

static void	process_unset_args(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (cmd->shell->env)
		{
			if (ft_strcmp(cmd->args[i], "PATH") == 0)
				cmd->shell->path_unset = true;
			remove_env_var(cmd, cmd->args[i]);
			if (ft_strcmp(cmd->args[i], "PATH") == 0
				&& get_env_value("PATH", cmd->shell->env))
				cmd->shell->path_unset = false;
		}
		i++;
	}
}

void	ft_unset(t_cmd *cmd)
{
	int	had_error;

	had_error = 0;
	if (!cmd->args[1])
	{
		cmd->shell->exit_status = 0;
		return ;
	}
	process_unset_args(cmd);
	if (had_error)
		cmd->shell->exit_status = 1;
	else
		cmd->shell->exit_status = 0;
}
