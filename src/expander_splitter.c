/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_splitter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/04/14 11:34:11 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 11:34:31 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/**
 * Split an expanded variable value into separate tokens by whitespace
 * Used when a variable is not quoted and needs to be split by IFS (whitespace)
 * Returns an array of tokens, or NULL if no splitting occurs or on error
 */
static int	count_words(char *value)
{
	int i = 0;
	int word_count = 0;

	while (value[i])
	{
		while (value[i] && ft_isspace(value[i]))
			i++;
		if (value[i])
			word_count++;
		while (value[i] && !ft_isspace(value[i]))
			i++;
	}
	return (word_count);
}

static char	**allocate_tokens(int word_count)
{
	char **tokens;

	if (word_count <= 1)
		return (NULL);
	tokens = malloc((word_count + 1) * sizeof(char *));
	if (!tokens)
		return (NULL);
	return (tokens);
}

static char	*extract_word(char *value, int start, int len)
{
	return (ft_substr(value, start, len));
}

static int	add_word_to_tokens(char **tokens, char *value, int *i,
		int *word_count)
{
	int start = *i;
	int len;

	while (value[*i] && !ft_isspace(value[*i]))
		(*i)++;
	len = *i - start;
	if (len > 0)
	{
		tokens[*word_count] = extract_word(value, start, len);
		if (!tokens[*word_count])
		{
			while (*word_count > 0)
				free(tokens[--(*word_count)]);
			free(tokens);
			return (0);
		}
		(*word_count)++;
	}
	return (1);
}

char	**split_expanded_variable(char *value)
{
	char **tokens;
	int i = 0;
	int word_count = 0;

	if (!value || !*value)
		return (NULL);
	word_count = count_words(value);
	tokens = allocate_tokens(word_count);
	if (!tokens)
		return (NULL);
	word_count = 0;
	while (value[i])
	{
		while (value[i] && ft_isspace(value[i]))
			i++;
		if (!add_word_to_tokens(tokens, value, &i, &word_count))
			return (NULL);
	}
	tokens[word_count] = NULL;
	return (tokens);
}
