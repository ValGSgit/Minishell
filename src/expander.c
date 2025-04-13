/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/13 15:03:18 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	if (src && *src)
		new_str = ft_strjoin(dest, src);
	else
		new_str = ft_strjoin(dest, "");
	free(dest);
	return (new_str);
}

/**
 * Processes a single argument, expanding any variables as needed
 * Returns the expanded string
 */
char *process_argument(char *arg, t_shell *shell)
{
    t_expander_state state;
    int i;
    char *value;

    ft_memset(&state, 0, sizeof(t_expander_state));
    state.result = ft_strdup("");
    if (!state.result || !arg)
        return (state.result);
    i = -1;
    while (arg[++i])
    {
        if (arg[i] == '\'' && !state.in_dquote)
            state.in_quote = !state.in_quote;
        else if (arg[i] == '"' && !state.in_quote)
            state.in_dquote = !state.in_dquote;
        else if (arg[i] == '$' && !state.in_quote)
        {
            value = expand_variable(arg, &i, shell);
            if (!state.is_heredoc || (value && *value))
                state.result = append_str(state.result, value);     
            free(value);
        }
        else
            state.result = append_str(state.result, (char[]){arg[i], '\0'});
    }
    return (state.result);
}

/**
 * Applies word splitting to expanded variables when not in quotes
 * Returns a new expanded arguments array
 */
static char **apply_word_splitting(char **args, t_shell *shell)
{
    int i;
    int j;
    int count;
    char **new_args;
    char **word_tokens;
    char *expanded;

    if (!args)
        return (NULL);

    // First count the maximum potential arguments after splitting
    count = 0;
    for (i = 0; args[i]; i++) {
        expanded = process_argument(args[i], shell);
        if (!expanded)
            continue;
            
        // Check if needs splitting (if it contains $ outside quotes)
        bool needs_splitting = false;
        bool in_quote = false;
        bool in_dquote = false;
        for (j = 0; args[i][j]; j++) {
            if (args[i][j] == '\'' && !in_dquote)
                in_quote = !in_quote;
            else if (args[i][j] == '"' && !in_quote)
                in_dquote = !in_dquote;
            else if (args[i][j] == '$' && !in_quote) {
                needs_splitting = true;
                break;
            }
        }
        
        if (needs_splitting) {
            word_tokens = split_expanded_variable(expanded);
            if (word_tokens) {
                for (j = 0; word_tokens[j]; j++)
                    count++;
                free_tokens(word_tokens);
            } else {
                count++;
            }
        } else {
            count++;
        }
        free(expanded);
    }

    // Allocate and build the new args array
    new_args = ft_calloc(count + 1, sizeof(char *));
    if (!new_args)
        return (args);  // Return original on error

    count = 0;
    for (i = 0; args[i]; i++) {
        expanded = process_argument(args[i], shell);
        if (!expanded)
            continue;
            
        // Check if needs splitting
        bool needs_splitting = false;
        bool in_quote = false;
        bool in_dquote = false;
        for (j = 0; args[i][j]; j++) {
            if (args[i][j] == '\'' && !in_dquote)
                in_quote = !in_quote;
            else if (args[i][j] == '"' && !in_quote)
                in_dquote = !in_dquote;
            else if (args[i][j] == '$' && !in_quote) {
                needs_splitting = true;
                break;
            }
        }
        
        if (needs_splitting) {
            word_tokens = split_expanded_variable(expanded);
            if (word_tokens) {
                for (j = 0; word_tokens[j]; j++)
                    new_args[count++] = word_tokens[j];
                free(word_tokens);  // Free only the array, not its contents
            } else {
                new_args[count++] = expanded;
                continue;  // Skip freeing expanded, we transferred ownership
            }
        } else {
            new_args[count++] = expanded;
            continue;  // Skip freeing expanded, we transferred ownership
        }
        free(expanded);  // Free if we made copies
    }
    new_args[count] = NULL;
    
    // Free original args array (but not its contents, they're either freed or transferred)
    free(args);
    return (new_args);
}

void	expander(t_cmd *cmd, t_shell *shell)
{
	char	*expanded;

	if (!cmd || !cmd->args)
		return ;
	// Fix path resolution logic - only apply path resolution to commands without paths
	// that are not builtins
	if (cmd->args[0] && 
		ft_strchr(cmd->args[0], '/') == NULL && 
		!is_builtin(cmd->args[0]))
	{
		expanded = resolve_path(cmd->args[0], shell->env);
		free(cmd->args[0]);
		cmd->args[0] = expanded;
	}
	
	// Apply word splitting to all arguments
	cmd->args = apply_word_splitting(cmd->args, shell);
}

/* Remove null or empty strings from args array */
static char	**ft_clean_args(char **args)
{
	int		i;
	int		j;
	char	**cleaned;

	i = 0;
	j = 0;
	if (!args)
		return (NULL);
	while (args[i])
		i++;
	cleaned = malloc(sizeof(char *) * (i + 1));
	i = 0;
	if (!cleaned)
		return (NULL);
	while (args[i])
	{
		if (args[i] && *args[i])
			cleaned[j++] = args[i];
		else
			free(args[i]);
		i++;
	}
	cleaned[j] = NULL;
	free(args);
	return (cleaned);
}

void	expand_nodes(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*node;

	node = cmd;
	shell->cmd = node;
	while (node)
	{
		expander(node, shell);
		node->shell = shell;
		if (node->args)
			node->args = ft_clean_args(node->args);
		node = node->next;
	}
}


// char	*append_str(char *dest, char *src)
// {
// 	char	*new_str;

// 	if (!dest)
// 	{
// 		if (src)
// 			return (ft_strdup(src));
// 		else
// 			return (ft_strdup(""));
// 	}
// 	if (src && *src)
// 		new_str = ft_strjoin(dest, src);
// 	else
// 		new_str = ft_strjoin(dest, "");
// 	free(dest);
// 	return (new_str);
// }

// char *process_argument(char *arg, t_shell *shell)
// {
//     t_expander_state state;
//     int i;
//     char *value;

//     ft_memset(&state, 0, sizeof(t_expander_state));
//     state.result = ft_strdup("");
//     if (!state.result || !arg)
//         return (state.result);
//     i = -1;
//     while (arg[++i])
//     {
//         if (arg[i] == '\'' && !state.in_dquote)
//             state.in_quote = !state.in_quote;
//         else if (arg[i] == '"' && !state.in_quote)
//             state.in_dquote = !state.in_dquote;
//         else if (arg[i] == '$' && !state.in_quote)
//         {
//             value = expand_variable(arg, &i, shell);
//             if (!state.is_heredoc || (value && *value))
//                 state.result = append_str(state.result, value);     
//             free(value);
//         }
//         else
//             state.result = append_str(state.result, (char[]){arg[i], '\0'});
//     }
//     return (state.result);
// }

// void	expander(t_cmd *cmd, t_shell *shell)
// {
// 	int		i;
// 	char	*expanded;

// 	if (!cmd || !cmd->args)
// 		return ;
// 	// Fix path resolution logic - only apply path resolution to commands without paths
// 	// that are not builtins
// 	if (cmd->args[0] && 
// 		ft_strchr(cmd->args[0], '/') == NULL && 
// 		!is_builtin(cmd->args[0]))
// 	{
// 		expanded = resolve_path(cmd->args[0], shell->env);
// 		free(cmd->args[0]);
// 		cmd->args[0] = expanded;
// 	}
// 	i = 0;
// 	while (cmd->args[i])
// 	{
// 		expanded = process_argument(cmd->args[i], shell);
// 		if (expanded)
// 		{
// 			free(cmd->args[i]);
// 			cmd->args[i] = expanded;
// 		}
// 		i++;
// 	}
// }

// /* Remove null or empty strings from args array */
// static char	**ft_clean_args(char **args)
// {
// 	int		i;
// 	int		j;
// 	char	**cleaned;

// 	i = 0;
// 	j = 0;
// 	if (!args)
// 		return (NULL);
// 	while (args[i])
// 		i++;
// 	cleaned = malloc(sizeof(char *) * (i + 1));
// 	i = 0;
// 	if (!cleaned)
// 		return (NULL);
// 	while (args[i])
// 	{
// 		if (args[i] && *args[i])
// 			cleaned[j++] = args[i];
// 		else
// 			free(args[i]);
// 		i++;
// 	}
// 	cleaned[j] = NULL;
// 	free(args);
// 	return (cleaned);
// }

// void	expand_nodes(t_cmd *cmd, t_shell *shell)
// {
// 	t_cmd	*node;

// 	node = cmd;
// 	shell->cmd = node;
// 	while (node)
// 	{
// 		expander(node, shell);
// 		node->shell = shell;
// 		if (node->args)
// 			node->args = ft_clean_args(node->args);
// 		node = node->next;
// 	}
// }
