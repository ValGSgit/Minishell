/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:21:08 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 19:21:08 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_to_env(char ***env, char *new_var)
{
	int		i;
	char	**new_env;

	if (!new_var)
		return ;
	i = 0;
	while ((*env)[i])
		i++;
	new_env = malloc((i + 2) * sizeof(char *));
	if (!new_env)
		return ;
	i = 0;
	while ((*env)[i])
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
}

char	*extract_var_name(char *arg)
{
	char	*equals_pos;
	char	*plus_pos;
	char	*var_name;
	int		name_len;

	equals_pos = ft_strchr(arg, '=');
	plus_pos = ft_strchr(arg, '+');
	if (plus_pos && plus_pos[1] == '=')
		name_len = plus_pos - arg;
	else if (equals_pos)
		name_len = equals_pos - arg;
	else
		name_len = ft_strlen(arg);
	var_name = ft_substr(arg, 0, name_len);
	return (var_name);
}

static char	*create_append_var(char *key, char *existing, char *append)
{
	char	*new_value;
	char	*new_var;
	char	*temp;

	new_value = ft_strjoin(existing, append);
	if (!new_value)
		return (NULL);
	new_var = ft_strjoin(key, "=");
	if (!new_var)
	{
		free(new_value);
		return (NULL);
	}
	temp = new_var;
	new_var = ft_strjoin(new_var, new_value);
	free(temp);
	free(new_value);
	return (new_var);
}

static char	*copy_without_plus(char *arg, char *plus_pos)
{
	char	*new_var;
	int		j;
	int		k;

	new_var = malloc(ft_strlen(arg));
	if (!new_var)
		return (NULL);
	j = 0;
	k = 0;
	while (arg[j])
	{
		if (j != (plus_pos - arg))
			new_var[k++] = arg[j];
		j++;
	}
	new_var[k] = '\0';
	return (new_var);
}

char	*handle_append_syntax(char *arg, char **env)
{
	char	*plus_pos;
	char	*equals_pos;
	char	*key;
	char	*existing_value;
	char	*value_to_append;
	char	*result;

	plus_pos = ft_strchr(arg, '+');
	equals_pos = ft_strchr(arg, '=');
	if (!plus_pos || !equals_pos || plus_pos + 1 != equals_pos)
		return (NULL);
	key = ft_substr(arg, 0, plus_pos - arg);
	if (!key)
		return (NULL);
	existing_value = get_env_value(key, env);
	value_to_append = equals_pos + 1;
	if (existing_value)
		result = create_append_var(key, existing_value, value_to_append);
	else
		result = copy_without_plus(arg, plus_pos);
	free(key);
	return (result);
}
