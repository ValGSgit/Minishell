/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_apply_split.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:35:22 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 16:06:56 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	needs_word_splitting(char *str)
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

static void	count_args_with_tokens(char *expanded, int *count, char ***word_tokens)
{
	int	j;

	*word_tokens = split_expanded_variable(expanded);
	if (*word_tokens)
	{
		j = 0;
		while ((*word_tokens)[j])
		{
			(*count)++;
			j++;
		}
		free_tokens(*word_tokens);
	}
	else
		(*count)++;
}

static int	count_args_after_splitting(char **args, t_shell *shell)
{
	int		i;
	int		count;
	char	**word_tokens;
	char	*expanded;

	count = 0;
	i = 0;
	while (args && args[i])
	{
		expanded = process_argument(args[i], shell);
		if (!expanded || !needs_word_splitting(args[i]))
			count++;
		else
			count_args_with_tokens(expanded, &count, &word_tokens);
		if (expanded)
			free(expanded);
		i++;
	}
	return (count);
}

static void	copy_word_tokens(char **word_tokens, char **new_args, int *count)
{
	int	j;

	j = 0;
	while (word_tokens[j])
	{
		new_args[(*count)++] = ft_strdup(word_tokens[j]);
		j++;
	}
	ft_free(word_tokens);
}

static void	process_arg_for_splitting(char *arg, char *expanded, int *count,
		char **new_args)
{
	char	**word_tokens;

	if (!expanded)
		return ;
	if (!needs_word_splitting(arg))
	{
		new_args[(*count)++] = expanded;
		return ;
	}
	word_tokens = split_expanded_variable(expanded);
	if (!word_tokens)
	{
		new_args[(*count)++] = expanded;
		return ;
	}
	copy_word_tokens(word_tokens, new_args, count);
	free(expanded);
}

static char	**build_args_after_splitting(char **args, int max_count,
		t_shell *shell)
{
	int		i;
	int		count;
	char	**new_args;
	char	*expanded;

	new_args = xmalloc((max_count + 1) * sizeof(char *));
	if (!new_args)
		return (args);
	ft_memset(new_args, 0, (max_count + 1) * sizeof(char *));
	count = 0;
	i = 0;
	while (args && args[i])
	{
		expanded = process_argument(args[i], shell);
		process_arg_for_splitting(args[i], expanded, &count, new_args);
		i++;
	}
	new_args[count] = NULL;
	ft_free(args);  // Use ft_free instead of free for better tracking
	return (new_args);
}

char	**apply_word_splitting(char **args, t_shell *shell)
{
	int		count;
	char	**new_args;

	if (!args)
		return (NULL);
	count = count_args_after_splitting(args, shell);
	new_args = build_args_after_splitting(args, count, shell);
	return (new_args);
}
