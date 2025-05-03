/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_extra_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iionescu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:23:28 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/02 16:23:32 by iionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_sorted_env(char **env)
{
	int		i;
	int		env_count;
	char	**env_copy;
	int		fd;

	fd = STDOUT_FILENO;
	if (!env)
		return ;
	env_count = 0;
	while (env[env_count])
		env_count++;
	env_copy = copy_env_array(env, env_count);
	if (!env_copy)
		return ;
	ft_sort_strings(env_copy, env_count);
	i = 0;
	while (env_copy[i])
	{
		print_env_var(env_copy[i], fd);
		i++;
	}
	free_env_copy(env_copy);
}
