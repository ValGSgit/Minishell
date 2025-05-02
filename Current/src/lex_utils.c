/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:13:39 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 10:24:35 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

int	is_redir_without_space(char **input)
{
	if (**input == '>' && !ft_isspace(*(*input + 1))
		&& !is_special_char(*(*input + 1)))
		return (1);
	else if (**input == '<' && !ft_isspace(*(*input + 1))
		&& !is_special_char(*(*input + 1)))
		return (1);
	else if (**input == '<' && *(*input + 1) == '<' && !ft_isspace(*(*input
				+ 2)) && !is_special_char(*(*input + 2)))
		return (2);
	else if (**input == '>' && *(*input + 1) == '>' && !ft_isspace(*(*input
				+ 2)) && !is_special_char(*(*input + 2)))
		return (2);
	else
		return (0);
}

/* Handles quotes and toggles the in_quote state */
int	handle_quotes(char **input, bool *in_quote, char *quote_char)
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
