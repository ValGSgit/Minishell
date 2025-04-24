/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:13:08 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 19:13:08 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	swap_strings(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort_env(char **env_copy, int env_count)
{
	int	i;
	int	j;
	int	swapped;

	i = 0;
	while (i < env_count - 1)
	{
		swapped = 0;
		j = 0;
		while (j < env_count - i - 1)
		{
			if (ft_strcmp(env_copy[j], env_copy[j + 1]) > 0)
			{
				swap_strings(&env_copy[j], &env_copy[j + 1]);
				swapped = 1;
			}
			j++;
		}
		if (swapped == 0)
			break ;
		i++;
	}
}

static void	print_env_var(char *var)
{
	char	*equals_pos;
	int		j;

	ft_putstr_fd("declare -x ", 1);
	equals_pos = ft_strchr(var, '=');
	if (equals_pos)
	{
		j = 0;
		while (var[j] != '=')
		{
			ft_putchar_fd(var[j], 1);
			j++;
		}
		ft_putstr_fd("=\"", 1);
		j++;
		while (var[j])
		{
			if (var[j] == '\"' || var[j] == '$' || 
				var[j] == '\\' || var[j] == '`')
				ft_putchar_fd('\\', 1);
			ft_putchar_fd(var[j], 1);
			j++;
		}
		ft_putstr_fd("\"", 1);
	}
	else
		ft_putstr_fd(var, 1);
	ft_putchar_fd('\n', 1);
}

void	print_sorted_env(char **env)
{
	int		i;
	int		env_count;
	char	**env_copy;

	if (!env)
		return ;
	env_count = 0;
	while (env[env_count])
		env_count++;
	env_copy = xmalloc((env_count + 1) * sizeof(char *));
	if (!env_copy)
		return ;
	i = 0;
	while (i < env_count)
	{
		env_copy[i] = ft_strdup(env[i]);
		i++;
	}
	env_copy[i] = NULL;
	sort_env(env_copy, env_count);
	i = 0;
	while (env_copy[i])
		print_env_var(env_copy[i++]);
	i = 0;
	while (env_copy[i])
		free(env_copy[i++]);
	free(env_copy);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}