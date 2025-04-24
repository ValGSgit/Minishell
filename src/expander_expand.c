/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 12:42:16 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char *handle_special_cases(char *str, int *i, t_shell *shell)
{
    int start = *i + 1;

    if (!str[start])
        return (ft_strdup("$"));
    if (str[start] == '?')
    {
        (*i)++;
        return (ft_itoa(shell->exit_status));
    }
    if (str[start] == ' ' || str[start] == '\t' || str[start] == '/' || str[start] == '$')
        return (ft_strdup("$"));
    if (ft_isdigit(str[start]))
    {
        (*i)++;
        return (ft_strdup(""));
    }
    // Handle empty quoted strings after $
    if ((str[start] == '"' && str[start+1] == '"') || 
        (str[start] == '\'' && str[start+1] == '\''))
    {
        *i += 2;  // Skip both quote chars
        return (ft_strdup(""));
    }
    return (NULL);
}

static char *handle_special_characters(char *str, int *i)
{
    int start = *i + 1;

    if (!ft_isalpha(str[start]) && str[start] != '_')
    {
        // If next char is double or single quote, extract just the quoted content
        if (str[start] == '"' || str[start] == '\'')
        {
            char quote_type = str[start];
            int quote_start = start + 1; // Start after the opening quote
            int end = quote_start;
            
            // Find the closing quote
            while (str[end] && str[end] != quote_type)
                end++;
                
            // If we found the closing quote
            if (str[end] == quote_type)
            {
                // Skip over the entire construct in the input
                *i = end + 1;
                
                // Extract just the content between quotes
                return (ft_substr(str, quote_start, end - quote_start));
            }
            return (ft_strdup("$"));
        }
        
        // For other special characters, return $ followed by the character
        (*i)++;
        char result[3] = {'$', str[start], '\0'};
        return (ft_strdup(result));
    }
    return (NULL);
}

static char *extract_variable_name(char *str, int *i)
{
    int start = *i + 1;
    int len = 0;

    while (str[start + len] && (ft_isalnum(str[start + len]) || str[start + len] == '_'))
        len++;
    *i += len;
    return (ft_substr(str, start, len));
}

static char *lookup_variable_value(char *var_name, t_shell *shell)
{
    char *value;

    if (!var_name || !*var_name)
    {
        xfree(var_name);
        return (ft_strdup("$"));
    }
    value = get_env_value(var_name, shell->env);
    xfree(var_name);
    if (!value)
        return (ft_strdup(""));
    return (ft_strdup(value));
}

char *expand_variable(char *str, int *i, t_shell *shell)
{
    char *special_case_result;
    char *special_char_result;
    char *var_name;

    special_case_result = handle_special_cases(str, i, shell);
    if (special_case_result)
        return (special_case_result);
    special_char_result = handle_special_characters(str, i);
    if (special_char_result)
        return (special_char_result);
    var_name = extract_variable_name(str, i);
    if (!var_name)
        return (ft_strdup(""));
    return (lookup_variable_value(var_name, shell));
}