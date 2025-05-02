/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_counter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:18:14 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 20:23:55 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_special_char(char *input)
{
	if (is_special_char(*input))
	{
		if ((*input == '>' && *(input + 1) == '>')
			|| (*input == '<' && *(input + 1) == '<'))
			return (1);
		return (1);
	}
	return (0);
}

static int	count_quotes(char **input, bool *in_quote, char *quote_char)
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

static int	count_tokens_processing(char *input)
{
	int		token_count;
	bool	in_quote;
	char	quote_char;
	char	*start;
	bool	was_empty_quote;

	token_count = 0;
	in_quote = false;
	quote_char = '\0';
	start = input;
	was_empty_quote = false;
	while (*input)
	{
		if (count_quotes(&input, &in_quote, &quote_char))
		{
			if (!in_quote && input - start == 2)
			{
				token_count++;
				start = input;
				was_empty_quote = true;
			}
			continue ;
		}
		if (was_empty_quote && (ft_isspace(*input)
				|| is_special_char(*input) || !*input))
		{
			start = input;
			was_empty_quote = false;
		}
		if (!in_quote && (ft_isspace(*input) || is_special_char(*input)))
		{
			if (start != input)
				token_count++;
			if (count_special_char(input))
				token_count++;
			start = input + 1;
		}
		input++;
	}
	if (start != input && !was_empty_quote)
		token_count++;
	return (token_count);
}

int	count_tokens(char *input)
{
	if (!input)
		return (0);
	return (count_tokens_processing(input) + 1);
}
