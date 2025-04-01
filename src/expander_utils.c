/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:02:58 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/01 12:09:41 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_missing_value(int *i, char *str, int start, int len, bool in_dquote)
{
	char	*value;

	if (in_dquote)
		value = ft_strdup("");
	else
	{
		value = ft_strdup("");
		*i += len;
		value = append_str(value, str + start + len);
	}
	return (value);
}

char	*expand_exit_status(int *i, t_shell *shell)
{
	char	*status;

	*i += 1;
	if (!shell->exit_status)
		status = ft_strdup("0");
	else
		status = ft_itoa(shell->exit_status);
	return (status);
}

char	*handle_empty_var_name(char *var_name)
{
	free(var_name);
	return (ft_strdup("$"));
}

char	*expand_variable(char *str, int *i, t_shell *shell, bool in_dquote)
{
	char	*var_name;
	char	*value;
	int		start;
	int		len;

	start = *i + 1;
	len = 0;
	if (!str[start])
		return (ft_strdup("$"));
	if (str[start] == '?')
		return (expand_exit_status(i, shell));
	while (str[start + len] && ft_isalnum(str[start + len]))
		len++;
	var_name = ft_substr(str, start, len);
	if (!var_name || !var_name[0])
		return (handle_empty_var_name(var_name));
	value = get_env_value(var_name, shell->env);
	free(var_name);
	if (!value)
		return (handle_missing_value(i, str, start, len, in_dquote));
	*i += len;
	return (ft_strdup(value));
}