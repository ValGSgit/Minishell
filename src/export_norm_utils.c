/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_norm_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:22:25 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 16:34:24 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_or_add_env(char *arg, char ***env)
{
	char	*var_name;

	if (var_is_too_large(arg))
	{
		var_name = extract_var_name(arg);
		handle_var_size_error(var_name);
		free(var_name);
		return ;
	}
	if (ft_strchr(arg, '+'))
	{
		if (handle_plus_syntax(arg, env))
			return ;
	}
	var_name = extract_var_name(arg);
	if (!var_name)
		return ;
	find_and_update_env(arg, var_name, env);
	free(var_name);
}

void	handle_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	process_export_arg(char *arg, t_cmd *cmd)
{
	char	*var_name;

	if (!arg)
		return (0);
	var_name = extract_var_name(arg);
	if (!is_valid_key(var_name))
	{
		free(var_name);
		handle_export_error(arg);
		cmd->shell->exit_status = 1;
		return (0);
	}
	free(var_name);
	if (ft_strchr(arg, '='))
	{
		update_or_add_env(arg, &cmd->shell->env);
		cmd->shell->exit_status = 0;
		return (1);
	}
	return (0);
}

void	ft_export(t_cmd *cmd)
{
	int	i;

	if (!cmd->args[1])
	{
		print_sorted_env(cmd->shell->env);
		cmd->shell->exit_status = 0;
		return ;
	}
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_key(cmd->args[i]))
		{
			handle_export_error(cmd->args[i]);
			cmd->shell->exit_status = 1;
		}
		else
			process_export_arg(cmd->args[i], cmd);
		i++;
	}
}
