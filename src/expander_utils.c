/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:02:58 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/13 16:21:16 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// static char	*handle_empty_var_name(char *var_name)
// {
// 	free(var_name);
// 	return (ft_strdup("$"));
// }

/**
 * Split an expanded variable value into separate tokens by whitespace
 * Used when a variable is not quoted and needs to be split by IFS (whitespace)
 * Returns an array of tokens, or NULL if no splitting occurs or on error
 */
char **split_expanded_variable(char *value)
{
    char **tokens = NULL;
    int i;
    int word_count;
    
    if (!value || !*value)
        return (NULL);
    
    // Count the number of words (separated by whitespace)
    word_count = 0;
    i = 0;
    while (value[i])
    {
        // Skip whitespace
        while (value[i] && ft_isspace(value[i]))
            i++;
        
        // If we found a non-whitespace character, count a word
        if (value[i])
            word_count++;
            
        // Skip to the end of the word
        while (value[i] && !ft_isspace(value[i]))
            i++;
    }
    
    // If there's only one word or no words, no splitting needed
    if (word_count <= 1)
        return (NULL);
    
    // Allocate array for tokens
    tokens = ft_calloc(word_count + 1, sizeof(char *));
    if (!tokens)
        return (NULL);
    
    // Extract words
    i = 0;
    word_count = 0;
    while (value[i])
    {
        int start;
        int len;
        
        // Skip whitespace
        while (value[i] && ft_isspace(value[i]))
            i++;
        
        // Mark start of word
        start = i;
        
        // Find end of word
        while (value[i] && !ft_isspace(value[i]))
            i++;
            
        // Calculate word length
        len = i - start;
        
        // If we found a word, extract it
        if (len > 0)
        {
            tokens[word_count] = ft_substr(value, start, len);
            if (!tokens[word_count])
            {
                // Cleanup on error
                while (word_count > 0)
                    free(tokens[--word_count]);
                free(tokens);
                return (NULL);
            }
            word_count++;
        }
    }
    
    tokens[word_count] = NULL;
    return (tokens);
}

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
