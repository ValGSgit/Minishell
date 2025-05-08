/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:26:33 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 16:26:47 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*join_path_parts(char **parts, int count)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("/");
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (parts[i])
		{
			temp = result;
			if (i > 0 && result[ft_strlen(result) - 1] != '/')
				result = ft_strjoin(result, "/");
			else
				result = ft_strdup(result);
			free(temp);
			temp = result;
			result = ft_strjoin(result, parts[i]);
			free(temp);
		}
		i++;
	}
	return (result);
}

static void	clean_path_token(char **parts, int i)
{
	int	j;

	if (ft_strcmp(parts[i], "..") == 0 && i > 0)
	{
		j = i - 1;
		while (j > 0 && (!parts[j] || ft_strcmp(parts[j], "..") == 0))
			j--;
		if (j >= 0 && parts[j])
		{
			free(parts[j]);
			parts[j] = NULL;
		}
	}
	else if (ft_strcmp(parts[i], ".") == 0)
	{
		free(parts[i]);
		parts[i] = NULL;
	}
}

char	*resolve_path_parts(char **parts, int count)
{
	int		i;
	char	*result;

	i = 0;
	while (i < count)
	{
		clean_path_token(parts, i);
		i++;
	}
	result = join_path_parts(parts, count);
	return (result);
}

char	*build_full_path(const char *current_pwd, const char *path)
{
	char	*temp;
	char	*full_path;

	if (path[0] == '/')
		return (ft_strdup(path));
	temp = ft_strjoin(current_pwd, "/");
	if (!temp)
		return (NULL);
	full_path = safe_strjoin(temp, (char *)path, 1);
	return (full_path);
}

void	update_logical_pwd(t_shell *shell, const char *path)
{
	char	*current_pwd;
	char	*new_pwd;
	char	*logical_path;

	current_pwd = get_env_value("PWD", shell->env);
	logical_path = calculate_logical_path(current_pwd, path);
	if (!logical_path)
		return ;
	new_pwd = ft_strjoin("PWD=", logical_path);
	free(logical_path);
	if (!new_pwd)
		return ;
	update_or_add_env(new_pwd, &shell->env);
	free(new_pwd);
}
