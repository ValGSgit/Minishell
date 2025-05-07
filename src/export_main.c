/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:20:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/05 11:33:32 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_env_entry(char ***env, char *arg, int i)
{
	free((*env)[i]);
	(*env)[i] = ft_strdup(arg);
	if (!(*env)[i])
	{
		perror("minishell: malloc");
		free_env(*env);
		free(arg);
	}
}

static void	find_and_update_env(char *arg, char *var_name, char ***env)
{
	int		i;
	int		found;
	char	*current_name;

	i = 0;
	found = 0;
	while ((*env)[i])
	{
		current_name = extract_var_name((*env)[i]);
		if (current_name && ft_strcmp(current_name, var_name) == 0)
		{
			update_env_entry(env, arg, i);
			found = 1;
			free(current_name);
			break ;
		}
		free(current_name);
		i++;
	}
	if (!found)
		add_to_env(env, ft_strdup(arg));
}

void	update_or_add_env(char *arg, char ***env)
{
	char	*var_name;
	char	*new_var;

	if (ft_strchr(arg, '+'))
	{
		new_var = handle_append_syntax(arg, *env);
		if (new_var)
		{
			update_or_add_env(new_var, env);
			free(new_var);
			return ;
		}
	}
	var_name = extract_var_name(arg);
	if (!var_name)
		return ;
	find_and_update_env(arg, var_name, env);
	free(var_name);
}

static void	handle_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
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
		else if (ft_strchr(cmd->args[i], '='))
		{
			update_or_add_env(cmd->args[i], &cmd->shell->env);
			cmd->shell->exit_status = 0;
		}
		i++;
	}
}
