/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:58:27 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 10:25:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Copy environment variables into a new array */
char	**copy_env_part1(int count)
{
	char	**env;

	if (count < 0)
		return (NULL);
	env = malloc(sizeof(char *) * (count + 1));
	if (!env)
		return (NULL);
	env[count] = NULL;
	return (env);
}

char	**copy_env(char **envp)
{
	int		i;
	int		count;
	char	**env;

	if (!envp)
		return (NULL);
	count = 0;
	while (envp[count])
		count++;
	env = copy_env_part1(count);
	if (!env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			free_env(env);
			return (NULL);
		}
		i++;
	}
	return (env);
}

char	*get_env_value(char *name, char **env)
{
	int	i;
	int	len;

	if (!name || !env)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	add_env_prepare(char ***env, char *new_entry, char **new_env)
{
	int	i;

	i = 0;
	while ((*env)[i])
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[i] = new_entry;
	new_env[i + 1] = NULL;
	safe_free(*env);
	*env = new_env;
}

void	add_to_env(char ***env, char *new_entry)
{
	char	**new_env;
	int		size;

	if (!new_entry)
		return ;
	size = 0;
	while ((*env)[size])
		size++;
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
	{
		safe_free(new_entry);
		return ;
	}
	add_env_prepare(env, new_entry, new_env);
}
