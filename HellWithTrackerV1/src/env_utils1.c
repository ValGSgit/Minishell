/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:41:32 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 19:05:32 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_env_vars(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (0);
	while (env[i])
		i++;
	return (i);
}

// static char	*format_env_var(const char *name, const char *value)
// {
// 	char	*formatted;
// 	size_t	len;

// 	if (!name)
// 		return (NULL);
// 	len = ft_strlen(name);
// 	if (value)
// 		len += ft_strlen(value) + 1;
// 	formatted = xmalloc(len + 1);
// 	if (!formatted)
// 		return (NULL);
// 	ft_strcpy(formatted, name);
// 	if (value)
// 	{
// 		ft_strlcat(formatted, "=", ft_strlen(formatted) + 1);
// 		ft_strlcat(formatted, value, ft_strlen(formatted) + ft_strlen(value) + 1);
// 	}
// 	return (formatted);
// }

char	**dup_env(char **env)
{
	int		i;
	int		count;
	char	**new_env;

	count = count_env_vars(env);
	new_env = xmalloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				xfree(new_env[i]);
			xfree(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	*get_env_value(char *name, char **env)
{
	int	i;
	int	name_len;

	if (!env || !name)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
			return (env[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

void	set_env_value(char *key, char *value, t_shell *shell)
{
	char	*new_entry;
	char	*tmp;
	int		i;

	if (!key || !shell || !shell->env)
		return ;
	
	// First, remove any existing entry with the same key
	i = 0;
	while (shell->env[i])
	{
		tmp = ft_substr(shell->env[i], 0, ft_strchr(shell->env[i], '=') - shell->env[i]);
		if (tmp && ft_strcmp(tmp, key) == 0)
		{
			xfree(tmp);
			break;
		}
		xfree(tmp);
		i++;
	}
	
	// Create the new entry
	if (value)
	{
		new_entry = ft_strjoin(key, "=");
		if (!new_entry)
			return ;
		tmp = new_entry;
		new_entry = ft_strjoin(new_entry, value);
		xfree(tmp);
		if (!new_entry)
			return ;
	}
	else
		new_entry = ft_strdup(key);
	
	// Add it to the environment
	update_or_add_env(new_entry, &(shell->env));
	xfree(new_entry);
}
