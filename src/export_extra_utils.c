/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_extra_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:57:44 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 16:21:39 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_env_var_too_large(char *var)
{
	int		len;

	len = 0;
	if (!var)
		return (0);
	len = ft_strlen(var);
	return (len > MAX_ENV_VAR_SIZE);
}

static void	print_env_var_safe(char *env_var, int fd)
{
	char	*equals_pos;

	if (!env_var)
		return ;
	if (is_env_var_too_large(env_var))
	{
		equals_pos = ft_strchr(env_var, '=');
		ft_putstr_fd("declare -x ", fd);
		if (equals_pos)
		{
			write(fd, env_var, equals_pos - env_var);
			ft_putstr_fd("=\"<large value>\"", fd);
		}
		else
			ft_putstr_fd(env_var, fd);
		ft_putstr_fd("\n", fd);
		return ;
	}
	print_env_var(env_var, fd);
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
	while (env && env[env_count])
		env_count++;
	env_copy = copy_env_array(env, env_count);
	if (!env_copy)
		return ;
	i = 0;
	while (env_copy[i])
	{
		print_env_var_safe(env_copy[i], fd);
		i++;
	}
	free_env_copy(env_copy);
}
