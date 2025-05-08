/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_extra_extra.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:57:27 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 16:33:55 by vagarcia         ###   ########.fr       */
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

bool	is_export_var_assignment(char **args, int arg_index)
{
	if (arg_index <= 0 || !args || !args[0])
		return (false);
	if (ft_strcmp(args[0], "export") == 0 && ft_strchr(args[arg_index], '='))
		return (true);
	return (false);
}

static int	count_tokens_for_arg(char *arg, t_shell *shell,
	char **args, int arg_index)
{
	int		j;
	int		token_count;
	char	**word_tokens;
	char	*expanded;

	token_count = 0;
	expanded = process_argument(arg, shell);
	if (!expanded || !needs_word_splitting(arg)
		|| is_export_var_assignment(args, arg_index))
		token_count = 1;
	else
	{
		word_tokens = split_expanded_variable(expanded);
		if (word_tokens)
		{
			j = 0;
			while (word_tokens[j++])
				token_count++;
			free_tokens(word_tokens);
		}
		else
			token_count = 1;
	}
	return (safe_free(expanded), token_count);
}

int	count_args_after_splitting(char **args, t_shell *shell)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (args && args[i])
	{
		count += count_tokens_for_arg(args[i], shell, args, i);
		i++;
	}
	return (count);
}
