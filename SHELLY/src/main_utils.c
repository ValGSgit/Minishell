/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:31:51 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 20:28:16 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_empty_input(char *input)
{
	int		i;
	bool	only_whitespace;

	if (!input)
		return (1);
	i = 0;
	only_whitespace = true;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
		{
			only_whitespace = false;
			break ;
		}
		i++;
	}
	if (only_whitespace)
		return (1);
	return (0);
}

static int	process_input(t_shell *shell, char **tokens)
{
	shell->cmd = parser(tokens, shell);
	free_tokens(tokens); // comment
	if (!shell->cmd)
		return (1);
	expand_nodes(shell->cmd, shell);
	if ((shell->cmd->args && shell->cmd->args[0]) || shell->cmd->redirs)
		executor(shell->cmd, shell);
	free_cmd(shell->cmd);
	shell->cmd = NULL;
	return (0);
}

int	handle_input(t_shell *shell, char *input)
{
	char	**tokens;
	int		result;

	if (check_empty_input(input))
	{
		safe_free(input);
		return (0);
	}
	if (*input)
		add_history(input);
	tokens = lexer(input);
	if (!tokens)
	{
		safe_free(input);
		return (0);
	}
	result = process_input(shell, tokens);
	//if (tokens)
	//	free_tokens(tokens);
	safe_free(input);
	if (result)
		return (0);
	return (0);
}
