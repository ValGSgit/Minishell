/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_counter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:18:14 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/03 16:04:16 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int unclosed_check(char *input)
// {
// 	int		i;
// 	bool	in_quote;
// 	char	quote_char;

// 	i = 0;
// 	if (!input)
// 		return (0);
// 	in_quote = false;
// 	while (input[i])
// 	{
// 		if (input[i] == '\'' || input[i] == '\"')
// 		{
// 			if (!in_quote)
// 			{
// 				in_quote = true;
// 				quote_char = input[i];
// 			}
// 			else if (input[i] == quote_char)
// 				in_quote = false;
// 		}
// 		i++;
// 	}
// 	return (in_quote);
// }
