/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 18:48:49 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*handle_special_cases(char *str, int *i, t_shell *shell)
{
	int	start;

	start = *i + 1;
	if (!str[start])
		return (ft_strdup("$"));
	if (str[start] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	if (str[start] == ' ' || str[start] == '\t' || str[start] == '/'
		|| str[start] == '$')
		return (ft_strdup("$"));
	if (ft_isdigit(str[start]))
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if ((str[start] == '"' && str[start + 1] == '"')
		|| (str[start] == '\'' && str[start + 1] == '\''))
	{
		*i += 2;
		return (ft_strdup(""));
	}
	return (NULL);
}

static char	*handle_special_characters(char *str, int *i)
{
	int		start;
	char	result[3];

	start = *i + 1;
	if (!str[start] || (!ft_isalpha(str[start]) && str[start] != '_'))
	{
		if (str[start] == '"' || str[start] == '\'')
			return (handle_quoted_content(str, start, i));
		if (str[start])
		{
			(*i)++;
			result[0] = '$';
			result[1] = str[start];
			result[2] = '\0';
			return (ft_strdup(result));
		}
		return (ft_strdup("$"));
	}
	return (NULL);
}

static char	*extract_variable_name(char *str, int *i)
{
	int	start;
	int	len;

	start = *i + 1;
	len = 0;
	while (str[start + len] && (ft_isalnum(str[start + len])
			|| str[start + len] == '_'))
		len++;
	*i += len;
	return (ft_substr(str, start, len));
}

static char	*lookup_variable_value(char *var_name, t_shell *shell)
{
	char	*value;

	if (!var_name || !*var_name)
	{
		free(var_name);
		return (ft_strdup("$"));
	}
	value = get_env_value(var_name, shell->env);
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*expand_variable(char *str, int *i, t_shell *shell)
{
	char	*special_case_result;
	char	*special_char_result;
	char	*var_name;
	char	*result;

	special_case_result = handle_special_cases(str, i, shell);
	if (special_case_result)
		return (special_case_result);
	special_char_result = handle_special_characters(str, i);
	if (special_char_result)
		return (special_char_result);
	var_name = extract_variable_name(str, i);
	if (!var_name)
		return (ft_strdup(""));
	result = lookup_variable_value(var_name, shell);
	return (result);
}
