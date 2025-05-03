/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_extra_extra.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iionescu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:13:39 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/02 16:13:42 by iionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	needs_word_splitting(char *str)
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
