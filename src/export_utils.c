/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:00:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 16:35:50 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*join_values(char *key, char *existing, char *to_append)
{
	char	*new_value;
	char	*new_var;
	char	*temp;

	new_value = ft_strjoin(existing, to_append);
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

static char	*create_var_without_plus(char *arg, char *plus_pos)
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
	char	*new_var;

	plus_pos = ft_strchr(arg, '+');
	equals_pos = ft_strchr(arg, '=');
	if (!plus_pos || !equals_pos || plus_pos + 1 != equals_pos)
		return (NULL);
	key = ft_substr(arg, 0, plus_pos - arg);
	if (!key)
		return (NULL);
	existing_value = get_env_value(key, env);
	if (existing_value)
		new_var = join_values(key, existing_value, equals_pos + 1);
	else
		new_var = create_var_without_plus(arg, plus_pos);
	free(key);
	return (new_var);
}

char	*extract_var_name(char *arg)
{
	char	*equals_pos;
	char	*plus_pos;
	char	*var_name;
	int		name_len;

	if (!arg)
		return (NULL);
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

int	is_valid_key(char *str)
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
