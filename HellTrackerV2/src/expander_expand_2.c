/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_expand_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:59:42 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 15:59:42 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*handle_special_cases(char *str, int *i, t_shell *shell);
static char	*handle_quoted_vars(char *str, int *i);
static char	*extract_variable_name(char *str, int *i);
static char	*lookup_variable_value(char *var_name, t_shell *shell);

char	*expand_variable(char *str, int *i, t_shell *shell)
{
	char	*special_case_result;
	char	*quoted_var_result;
	char	*var_name;
	int		start_pos;

	start_pos = *i;
	(*i)++;  /* Skip the $ character */
	
	/* Handle $ at end of string */
	if (!str[*i])
		return (ft_strdup("$"));
		
	special_case_result = handle_special_cases(str, i, shell);
	if (special_case_result)
		return (special_case_result);
	
	quoted_var_result = handle_quoted_vars(str, i);
	if (quoted_var_result)
		return (quoted_var_result);
	
	var_name = extract_variable_name(str, i);
	if (!var_name || !*var_name)
	{
		xfree(var_name);
		if (*i == start_pos + 1)  /* No variable name extracted */
			return (ft_strdup("$"));
		return (ft_strdup(""));
	}
	
	return (lookup_variable_value(var_name, shell));
}

static char	*handle_special_cases(char *str, int *i, t_shell *shell)
{
	/* Handle $? - exit status */
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	
	/* Handle digits (positional parameters) - not supported */
	if (ft_isdigit(str[*i]))
	{
		(*i)++;
		return (ft_strdup(""));
	}
	
	/* Handle special characters */
	if (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '/'
		|| str[*i] == '$' || str[*i] == '=')
		return (ft_strdup("$"));
	
	return (NULL);
}

static char	*handle_quoted_vars(char *str, int *i)
{
	/* Handle $"" - empty string */
	if (str[*i] == '"' && str[*i + 1] == '"')
	{
		*i += 2;
		return (ft_strdup(""));
	}
	
	/* Handle $'' - empty string */
	if (str[*i] == '\'' && str[*i + 1] == '\'')
	{
		*i += 2;
		return (ft_strdup(""));
	}
	
	return (NULL);
}

static char	*extract_variable_name(char *str, int *i)
{
	int		start;
	int		len;

	start = *i;
	len = 0;
	/* Variable names must start with a letter or underscore */
	if (!ft_isalpha(str[start]) && str[start] != '_')
		return (NULL);
	
	/* Variable names can contain letters, digits, and underscores */
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
		xfree(var_name);
		return (ft_strdup(""));
	}
	
	value = get_env_value(var_name, shell->env);
	xfree(var_name);
	
	if (!value)
		return (ft_strdup(""));
	
	return (ft_strdup(value));
}