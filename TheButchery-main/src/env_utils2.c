/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:58:27 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/08 17:18:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**copy_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			free_env(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

char *get_env_value(char *name, char **env)
{
    int i;
    int len;

    if (!name || !env)
        return (NULL);
    len = ft_strlen(name);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
            return (env[i] + len + 1); // Return pointer to value part
        i++;
    }
    return (NULL);
}

