/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:02:58 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 12:30:52 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// static char	*handle_empty_var_name(char *var_name)
// {
// 	free(var_name);
// 	return (ft_strdup("$"));
// }

char *expand_variable(char *str, int *i, t_shell *shell)
{
    char *var_name;
    char *value;
    int start;
    int len;
    
    start = *i + 1;
    
    // Handle special cases
    if (!str[start])
        return (ft_strdup("$"));
        
    // Handle $? (exit status)
    if (str[start] == '?')
    {
        (*i)++;
        return (ft_itoa(shell->exit_status));
    }
    
    // Handle $ followed by a space or another special character
    if (str[start] == ' ' || str[start] == '\t' || str[start] == '/' || 
        str[start] == '$')
    {
        return (ft_strdup("$"));
    }
    
    // Handle $0, $1, etc. positional parameters
    if (ft_isdigit(str[start]))
    {
        (*i)++;
        return (ft_strdup(""));
    }
    
    // Handle special characters after $
    if (!ft_isalpha(str[start]) && str[start] != '_')
    {
        if (str[start] != '"' && str[start] != '\'')
        {
            (*i)++;
            char result[3] = {'$', str[start], '\0'};
            return (ft_strdup(result));
        }
        return (ft_strdup("$"));
    }
    
    // Extract variable name
    len = 0;
    while (str[start + len] && 
           (ft_isalnum(str[start + len]) || str[start + len] == '_'))
        len++;
        
    var_name = ft_substr(str, start, len);
    if (!var_name || !*var_name)
    {
        free(var_name);
        return (ft_strdup("$"));
    }
    
    // Lookup variable in environment
    value = get_env_value(var_name, shell->env);
    free(var_name);
    
    *i += len;
    
    // Return empty string for undefined variables
    if (!value)
        return (ft_strdup(""));
        
    return (ft_strdup(value));
}
