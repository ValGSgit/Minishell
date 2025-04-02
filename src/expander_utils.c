/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:02:58 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/02 14:54:57 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_exit_status(int *i, t_shell *shell)
{
	char	*status;

	*i += 1;
	if (!shell->exit_status)
		status = ft_strdup("0");
	else
		status = ft_itoa(shell->exit_status);
	return (status);
}

static char	*handle_empty_var_name(char *var_name)
{
	free(var_name);
	return (ft_strdup("$"));
}

static char	*handle_miss(int *i, char *str, int strt_len[2], bool in_dquote)
{
	char	*value;

	if (in_dquote)
		value = ft_strdup("");
	else
	{
		value = ft_strdup("");
		*i += strt_len[1];
		value = append_str(value, str + strt_len[0] + strt_len[1]);
	}
	return (value);
}

char	*expand_variable(char *str, int *i, t_shell *shell, bool in_dquote)
{
	char	*var_name;
	char	*value;
	int		strt_len[2];

	strt_len[0] = *i + 1;
	strt_len[1] = 0;
	if (!str[strt_len[0]])
		return (ft_strdup("$"));
	if (str[strt_len[0]] == '?')
		return (expand_exit_status(i, shell));
	while (str[strt_len[0] + strt_len[1]] 
		&& ft_isalnum(str[strt_len[0] + strt_len[1]]))
		strt_len[1]++;
	var_name = ft_substr(str, strt_len[0], strt_len[1]);
	if (!var_name || !var_name[0])
		return (handle_empty_var_name(var_name));
	value = get_env_value(var_name, shell->env);
	free(var_name);
	if (!value)
	{
		*i += strt_len[1];
		return (handle_miss(i, str, strt_len, in_dquote));
	}
	*i += strt_len[1];
	return (ft_strdup(value));
}
