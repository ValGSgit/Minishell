/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_apply_split.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:35:22 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 12:44:16 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Checks if a string needs word splitting (contains $ outside quotes)
 */
static bool	needs_word_splitting(char *str)
{
	int		i;
	bool	in_quote;
	bool	in_dquote;

	in_quote = false;
	in_dquote = false;
	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (str[i] == '"' && !in_quote)
			in_dquote = !in_dquote;
		else if (str[i] == '$' && !in_quote)
			return (true);
		i++;
	}
	return (false);
}

/**
 * Counts how many args will be in the result after word splitting
 */
static int	count_args_after_splitting(char **args, t_shell *shell)
{
	int		i;
	int		j;
	int		count;
	char	**word_tokens;
	char	*expanded;

	count = 0;
	i = 0;
	while (args && args[i])
	{
		expanded = process_argument(args[i], shell);
		if (!expanded || !needs_word_splitting(args[i]))
			count++;
		else if ((word_tokens = split_expanded_variable(expanded)))
		{
			j = 0;
			while (word_tokens[j])
			{
				count++;
				free(word_tokens[j]);
				j++;
			}
			free(word_tokens); // fixed expansion leak
		}
		else
			count++;
		free(expanded);
		i++;
	}
	return (count);
}

/**
 * Process one argument for word splitting
 */
static void	process_arg_for_splitting(char *arg, char *expanded, int *count,
		char **new_args)
{
	char	**word_tokens;
	int		j;

	if (!expanded)
		return ;
	if (!needs_word_splitting(arg))
	{
		new_args[(*count)++] = expanded;
		return ;
	}
	word_tokens = split_expanded_variable(expanded);
	if (!word_tokens)
	{
		new_args[(*count)++] = expanded;
		return ;
	}
	j = 0;
	while (word_tokens[j])
		new_args[(*count)++] = word_tokens[j++];
	free(word_tokens);
	free(expanded);
}

/**
 * Build the new arguments array after word splitting
 */
static char	**build_args_after_splitting(char **args, int max_count,
		t_shell *shell)
{
	int		i;
	int		count;
	char	**new_args;
	char	*expanded;

	new_args = ft_calloc(max_count + 1, sizeof(char *));
	if (!new_args)
		return (args);
	count = 0;
	i = 0;
	while (args && args[i])
	{
		expanded = process_argument(args[i], shell);
		process_arg_for_splitting(args[i], expanded, &count, new_args);
		i++;
	}
	i = 0;
    while (args && args[i])
    {
        free(args[i]);  // Free each dynamically allocated string in 'args'
        i++;
    }
    free(args); //fixed exit leak
	return (new_args);
}

/**
 * Applies word splitting to expanded variables when not in quotes
 * Returns a new expanded arguments array
 */
char	**apply_word_splitting(char **args, t_shell *shell)
{
	int		count;
	char	**new_args;

	if (!args)
		return (NULL);
	count = count_args_after_splitting(args, shell);
	new_args = build_args_after_splitting(args, count, shell);
	return (new_args);
}
