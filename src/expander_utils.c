/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:40:32 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 19:08:49 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_quoted_content(char *str, int start, int *i)
{
	char	quote_type;
	int		quote_start;
	int		end;

	quote_type = str[start];
	quote_start = start + 1;
	end = quote_start;
	while (str[end] && str[end] != quote_type)
		end++;
	if (str[end] == quote_type)
	{
		*i = end;
		return (ft_substr(str, quote_start, end - quote_start));
	}
	return (ft_strdup("$"));
}

char	*append_str(char *dest, char *src)
{
	char	*new_str;

	if (!dest)
	{
		if (src)
			return (ft_strdup(src));
		else
			return (ft_strdup(""));
	}
	if (!src)
		return (dest);
	new_str = safe_strjoin(dest, src, 1);
	return (new_str);
}

char	*resolve_path_in_current_dir(char *cmd)
{
	char	*path;
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (ft_strdup(cmd));
	path = safe_strjoin(current_dir, "/", 1);
	if (!path)
		return (ft_strdup(cmd));
	return (safe_strjoin(path, cmd, 1));
}

void	resolve_non_builtin_path(t_cmd *node, t_shell *shell)
{
	char	*reresolved;

	if (node->args && node->args[0] && ft_strchr(node->args[0], '/') == NULL
		&& !is_builtin(node->args[0]))
	{
		if (shell->env && get_env_value("PATH", shell->env))
			reresolved = resolve_path(node->args[0], shell->env);
		else
			reresolved = resolve_path_in_current_dir(node->args[0]);
		if (reresolved)
		{
			free(node->args[0]);
			node->args[0] = reresolved;
		}
	}
}
