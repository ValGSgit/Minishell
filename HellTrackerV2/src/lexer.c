/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:56:16 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 22:30:12 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Processes special characters like |, >, >>, <, << */
static int	process_special_char(char **tokens, int *count, char **input)
{
	if (is_special_char(**input))
	{
		if ((**input == '>' && *(*input + 1) == '>') || (**input == '<'
				&& *(*input + 1) == '<'))
		{
			tokens[(*count)++] = ft_substr(*input, 0, 2);
			if (!tokens[(*count) - 1])
				return (-1);
			(*input) += 2;
			return (1);
		}
		tokens[(*count)++] = ft_substr(*input, 0, 1);
		if (!tokens[(*count) - 1])
			return (-1);
		(*input)++;
		return (1);
	}
	return (0);
}

/* Adds a token to the tokens array */
static int	add_token(char **tokens, int *count, char *start, char *end)
{
	char	*token;

	if (start == end)
		return (0);
	token = ft_substr(start, 0, end - start);
	if (!token)
		return (-1);
	tokens[(*count)++] = token;
	return (0);
}

/* Processes the input string and tokenizes it */
static int	process_input(char **input, t_lexer *lx, char **tokens)
{
	while (**input && lx->tokcount < MAX_TOKENS - 1)
	{
		if (handle_quotes(input, &lx->in_quote, &lx->quote_char))
			continue ;
		if (!lx->in_quote && (ft_isspace(**input) || is_special_char(**input)))
		{
			if (lx->start != *input && add_token(tokens, &lx->tokcount,
					lx->start, *input) < 0)
				return (-1);
			if (process_special_char(tokens, &lx->tokcount, input) == -1)
				return (-1);
			else if (process_special_char(tokens, &lx->tokcount, input) == 1)
			{
				lx->start = *input;
				continue ;
			}
			lx->start = *input + 1;
		}
		(*input)++;
	}
	return (0);
}

/* Finalizes tokenization by adding the last token if necessary */
static int	finalize_tokens(t_lexer lx, char *input, char **tokens)
{
	if (lx.start != input)
	{
		if (add_token(tokens, &lx.tokcount, lx.start, input) < 0)
			return (0);
	}
	tokens[lx.tokcount] = NULL;
	return (1);
}

/* Main lexer function */
char	**lexer(char *input)
{
	char	**tokens;
	t_lexer	lx;

	if (!input)
		return (NULL);
	tokens = xmalloc(MAX_TOKENS * sizeof(char *));
	if (!tokens)
		return (NULL);
	ft_bzero(tokens, MAX_TOKENS * sizeof(char *));
	ft_memset(&lx, 0, sizeof(t_lexer));
	lx.start = input;
	if (process_input(&input, &lx, tokens) < 0)
	{
		free_tokens(tokens);
		return (NULL);
	}
	if (!finalize_tokens(lx, input, tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
