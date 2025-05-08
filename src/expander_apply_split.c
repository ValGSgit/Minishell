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

static bool	is_export_var_assignment(char **args, int arg_index)
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
	if (expanded)
		free(expanded);
	return (token_count);
}

static int	count_args_after_splitting(char **args, t_shell *shell)
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

static void	process_arg_for_splitting(char *arg, char *expanded, int *count,
		char **new_args, char **orig_args, int arg_index)
{
	char	**word_tokens;
	int		j;
	bool	has_spaces;

	if (!expanded)
		return ;
	has_spaces = ft_strchr(expanded, ' ') != NULL;
	if (!needs_word_splitting(arg) || !has_spaces
		|| is_export_var_assignment(orig_args, arg_index))
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
	j = 0;
	while (word_tokens[j])
	{
		new_args[(*count)++] = ft_strdup(word_tokens[j]);
		j++;
	}
	free_tokens(word_tokens);
	free(expanded);
}

static char	**build_args_after_splitting(char **args, int max_count,
		t_shell *shell)
{
	int		i;
	int		count;
	char	**new_args;
	char	*expanded;

	new_args = malloc((max_count + 1) * sizeof(char *));
	if (!new_args)
		return (args);
	count = 0;
	i = 0;
	while (args && args[i])
	{
		expanded = process_argument(args[i], shell);
		process_arg_for_splitting(args[i], expanded, &count, new_args, args, i);
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
