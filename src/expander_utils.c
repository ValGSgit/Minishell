/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:02:58 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 10:17:40 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static char	*handle_empty_var_name(char *var_name)
{
	free(var_name);
	return (ft_strdup("$"));
}

// static char *handle_quoted_variable(char *str, int *i)
// {
//     int start;
//     int j;
//     char *content;
    
//     // Skip $ and opening quote
//     start = *i + 2;
//     j = start;
    
//     // Find closing quote
//     while (str[j] && str[j] != '"')
//         j++;
        
//     // Extract content between quotes
//     content = ft_substr(str, start, j - start);
    
//     // Update position
//     if (str[j] == '"')
//         *i = j;
//     else
//         *i = j - 1;
//     return content;
// }

// char *expand_variable(char *str, int *i, t_shell *shell, bool in_dquote)
// {
//     char *var_name;
//     char *value;
//     int strt_len[2];
    
//     // Check for $"string" pattern - bash treats this differently
//     if (in_dquote && str[*i + 1] == '"')
//         return (handle_quoted_variable(str, i));
    
//     strt_len[0] = *i + 1;
//     strt_len[1] = 0;
    
//     // Handle empty variable
//     if (!str[strt_len[0]])
//         return (ft_strdup("$"));
    
//     // Handle $? special variable
//     if (str[strt_len[0]] == '?')
//     {
//         (*i)++;
//         return (ft_itoa(shell->exit_status));
//     }
//     // Find variable name length
//     while (str[strt_len[0] + strt_len[1]] && 
//            (ft_isalnum(str[strt_len[0] + strt_len[1]]) || 
//             str[strt_len[0] + strt_len[1]] == '_'))
//         strt_len[1]++;
    
//     // Extract variable name
//     var_name = ft_substr(str, strt_len[0], strt_len[1]);
//     if (!var_name || !var_name[0])
//         return (handle_empty_var_name(var_name));
    
//     // Get variable value
//     value = get_env_value(var_name, shell->env);
//     free(var_name);
//     *i += strt_len[1];
    
//     // Handle case where variable doesn't exist
//     if (!value)
//         return (ft_strdup(""));
        
//     return (ft_strdup(value));
// }

char *expand_variable(char *str, int *i, t_shell *shell)
{
    char *var_name;
    char *value;
    int strt_len[2];
    
    // Check for $"string" pattern - this is a locale translation in bash
    // But we can just return the string inside the quotes
    if (str[*i + 1] == '"')
    {
        (*i)++; // Skip the $ but leave the quotation mark to be processed normally
        return ft_strdup(""); // Return empty string, let the normal quote handling take over
    }
    strt_len[0] = *i + 1;
    strt_len[1] = 0;
    // Handle empty variable
    if (!str[strt_len[0]])
        return (ft_strdup("$"));
    // Handle $? special variable
    if (str[strt_len[0]] == '?')
    {
        (*i)++;
        return (ft_itoa(shell->exit_status));
    }
    // Find variable name length
    while (str[strt_len[0] + strt_len[1]] && 
           (ft_isalnum(str[strt_len[0] + strt_len[1]]) || 
            str[strt_len[0] + strt_len[1]] == '_'))
        strt_len[1]++;
    // Extract variable name
    var_name = ft_substr(str, strt_len[0], strt_len[1]);
    if (!var_name || !var_name[0])
        return (handle_empty_var_name(var_name));
    // Get variable value
    value = get_env_value(var_name, shell->env);
    free(var_name);
    *i += strt_len[1];
    // Handle case where variable doesn't exist
    if (!value)
        return (ft_strdup(""));
    return (ft_strdup(value));
}