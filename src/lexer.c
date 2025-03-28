/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:56:16 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/28 13:30:02 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

/* Handles quotes and toggles the in_quote state */
static int	handle_quotes(char **input, bool *in_quote, char *quote_char)
{
	if (!*in_quote && (**input == '\'' || **input == '\"'))
	{
		*in_quote = true;
		*quote_char = **input;
		(*input)++;
		return (1);
	}
	else if (*in_quote && **input == *quote_char)
	{
		*in_quote = false;
		*quote_char = '\0';
		(*input)++;
		return (1);
	}
	return (0);
}

/* Processes special characters like |, >, >>, <, << */
static int	process_special_char(char **tokens, int *count, char **input)
{
	if (is_special_char(**input))
	{
		if ((**input == '>' && *(*input + 1) == '>') || (**input == '<'
				&& *(*input + 1) == '<'))
		{
			tokens[(*count)++] = ft_substr(*input, 0, 2);
			(*input) += 2;
			return (1);
		}
		tokens[(*count)++] = ft_substr(*input, 0, 1);
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
			if (process_special_char(tokens, &lx->tokcount, input))
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
char	**lexer(char *input, t_shell *shell)
{
	char **tokens;
	t_lexer lx;

	(void)shell; // Shell is unused since we are not expanding variables here
	tokens = ft_calloc(MAX_TOKENS, sizeof(char *));
	if (!tokens || !input)
		return (free(tokens), NULL);
	lx = (t_lexer){0};
	lx.start = input;
	if (process_input(&input, &lx, tokens) < 0)
		return (free_tokens(tokens), NULL);
	if (!finalize_tokens(lx, input, tokens))
		return (free_tokens(tokens), NULL);
	return (tokens);
}