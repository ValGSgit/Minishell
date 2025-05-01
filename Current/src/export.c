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

// static void	swap_strings(char **a, char **b)
// {
// 	char	*temp;

// 	temp = *a;
// 	*a = *b;
// 	*b = temp;
// }

void	ft_sort_strings(char **arr, int count)
{
	int	i;
	int	j;
	int	swapped;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		swapped = 0;
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				swapped = 1;
			}
			j++;
		}
		if (swapped == 0)
			break ;
		i++;
	}
}

static char	**copy_env_array(char **env, int env_count)
{
	char	**env_copy;
	int		i;

	env_copy = malloc((env_count + 1) * sizeof(char *));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (i < env_count)
	{
		env_copy[i] = ft_strdup(env[i]);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

static void	free_env_copy(char **env_copy)
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

static void	print_env_var(char *env_var, int fd)
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
		ft_putstr_fd("=\"", fd);
		j++;
		while (env_var[j])
		{
			if (env_var[j] == '\"' || env_var[j] == '$' || env_var[j] == '\\' 
						|| env_var[j] == '`')
				ft_putchar_fd('\\', fd);
			ft_putchar_fd(env_var[j], fd);
			j++;
		}
		ft_putstr_fd("\"", fd);
	}
	else
		ft_putstr_fd(env_var, fd);
	ft_putchar_fd('\n', fd);
}

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
