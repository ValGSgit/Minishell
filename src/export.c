/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:35:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 12:55:07 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**copy_env_array(char **env, int env_count)
{
	char	**env_copy;
	int		i;

	if (!env || env_count <= 0)
		return (NULL);
	env_copy = malloc((env_count + 1) * sizeof(char *));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (i < env_count)
	{
		if (env[i])
			env_copy[i] = ft_strdup(env[i]);
		else
			env_copy[i] = NULL;
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

void	free_env_copy(char **env_copy)
{
	int	i;

	i = 0;
	while (env_copy[i])
	{
		free(env_copy[i]);
		i++;
	}
	free(env_copy);
}

static void	print_env_var_logic(const char *env_var, int start_index, int fd)
{
	ft_putstr_fd("=\"", fd);
	while (env_var[start_index])
	{
		if (env_var[start_index] == '\"' || env_var[start_index] == '$'
			|| env_var[start_index] == '\\' || env_var[start_index] == '`')
			ft_putchar_fd('\\', fd);
		ft_putchar_fd(env_var[start_index], fd);
		start_index++;
	}
	ft_putstr_fd("\"", fd);
}

void	print_env_var(char *env_var, int fd)
{
	char	*equals_pos;
	int		j;

	ft_putstr_fd("declare -x ", fd);
	equals_pos = ft_strchr(env_var, '=');
	if (equals_pos)
	{
		j = 0;
		while (env_var[j] != '=')
		{
			ft_putchar_fd(env_var[j], fd);
			j++;
		}
		print_env_var_logic(env_var, j + 1, fd);
	}
	else
		ft_putstr_fd(env_var, fd);
	ft_putchar_fd('\n', fd);
}
