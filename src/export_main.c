/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:20:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 16:35:44 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_var_size_error(char *var_name)
{
	ft_putstr_fd("minishell: export: `", 2);
	if (var_name)
		ft_putstr_fd(var_name, 2);
	ft_putstr_fd("': value too large (exceeds size limit)\n", 2);
}

int	var_is_too_large(char *arg)
{
	return (arg && ft_strlen(arg) > MAX_ENV_VAR_SIZE);
}

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

void	find_and_update_env(char *arg, char *var_name, char ***env)
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

int	handle_plus_syntax(char *arg, char ***env)
{
	char	*new_var;
	char	*var_name;

	new_var = handle_append_syntax(arg, *env);
	if (!new_var)
		return (1);
	if (!var_is_too_large(new_var))
		update_or_add_env(new_var, env);
	else
	{
		var_name = extract_var_name(new_var);
		handle_var_size_error(var_name);
		free(var_name);
	}
	free(new_var);
	return (1);
}
