/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_extra_extra.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:57:27 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/07 14:56:27 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	needs_word_splitting(char *str)
{
	int		i;
	bool	in_quote;
	bool	in_dquote;
	bool	found_var;

	in_quote = false;
	in_dquote = false;
	found_var = false;
	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (str[i] == '"' && !in_quote)
			in_dquote = !in_dquote;
		else if (str[i] == '$' && (!in_quote))
			found_var = true;
		i++;
	}
	return (found_var && !in_quote && !in_dquote);
}
