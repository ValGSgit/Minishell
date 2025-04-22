/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:30:31 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 19:30:31 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	find_and_update(char *var_name, char *arg, char ***env)
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
			free((*env)[i]);
			(*env)[i] = ft_strdup(arg);
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
	find_and_update(var_name, arg, env);
	free(var_name);
}