/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_apply_split.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:35:22 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/07 15:55:27 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	add_word_tokens_to_args(char **word_tokens, char **new_args,
	int *count, char *expanded)
{
	int	j;

	if (!word_tokens || !new_args || !count)
	{
		if (expanded)
			free(expanded);
		return ;
	}
	j = 0;
	while (word_tokens[j])
	{
		new_args[(*count)++] = ft_strdup(word_tokens[j]);
		j++;
	}
	free_tokens(word_tokens);
	free(expanded);
}

static void	process_arg_for_splitting(char *arg, char **orig_args,
	t_split_context *ctx)
{
	char	**word_tokens;
	char	*expanded;
	bool	has_spaces;

	expanded = ctx->expanded;
	if (!expanded || !ctx->new_args)
		return ;
	has_spaces = ft_strchr(expanded, ' ') != NULL;
	if (!needs_word_splitting(arg) || !has_spaces
		|| is_export_var_assignment(orig_args, ctx->arg_index))
	{
		ctx->new_args[(*(ctx->count))++] = expanded;
		return ;
	}
	word_tokens = split_expanded_variable(expanded);
	if (!word_tokens)
	{
		ctx->new_args[(*(ctx->count))++] = expanded;
		return ;
	}
	add_word_tokens_to_args(word_tokens, ctx->new_args, ctx->count, expanded);
}

static char	**build_args_after_splitting(char **args, int max_count,
		t_shell *shell)
{
	int				i;
	int				count;
	char			**new_args;
	t_split_context	ctx;

	new_args = malloc((max_count + 1) * sizeof(char *));
	if (!new_args)
		return (args);
	count = 0;
	i = 0;
	while (args && args[i])
	{
		ctx.expanded = process_argument(args[i], shell);
		ctx.new_args = new_args;
		ctx.count = &count;
		ctx.arg_index = i;
		process_arg_for_splitting(args[i], args, &ctx);
		i++;
	}
	new_args[count] = NULL;
	if (args)
		free_ptr_array((void **)args);
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
