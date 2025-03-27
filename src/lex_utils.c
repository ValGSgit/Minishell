/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:13:39 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/24 14:18:32 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

void	*end_check(t_lexer lx, char *input, char **tokens, t_shell *shell)
{
    // If there is remaining input, add it as a token
    if (lx.start != input)
    {
        if (add_token(tokens, &lx.tokcount, lx.start, input, shell) < 0)
        {
            free_tokens(tokens);
            return (NULL);
        }
    }
    // Null-terminate the tokens array
    tokens[lx.tokcount] = NULL;
    return (tokens);
}

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
    if (**input == '>' && !ft_isspace(*(*input + 1)) && !is_special_char(*(*input + 1)))
        return (1);
    else if (**input == '<' && !ft_isspace(*(*input + 1)) && !is_special_char(*(*input + 1)))
        return (1);
    else if (**input == '<' && *(*input + 1) == '<' && !ft_isspace(*(*input + 2)) && !is_special_char(*(*input + 2)))
        return (2);
    else if (**input == '>' && *(*input + 1) == '>' && !ft_isspace(*(*input + 2)) && !is_special_char(*(*input + 2)))
        return (2);
    else 
        return (0);
}