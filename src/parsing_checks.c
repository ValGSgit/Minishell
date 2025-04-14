/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:09:29 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 11:17:12 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_redir_errors(char **tokens, t_shell *shell, int *i)
{
	if (is_redirection(tokens[*i]))
	{
		if (!tokens[*i + 1])
		{
			handle_syntax_error("newline", shell);
			return (1);
		}
		if (is_metacharacter(tokens[*i + 1]))
		{
			handle_syntax_error(tokens[*i + 1], shell);
			return (1);
		}
	}
	return (0);
}

int	check_quotes(char *token, t_shell *shell)
{
	char	quote;
	int		j;

	if (token[0] == '"' || token[0] == '\'')
	{
		quote = token[0];
		j = 1;
		while (token[j] && token[j] != quote)
			j++;
		if (!token[j])
		{
			handle_syntax_error("unclosed quote", shell);
			return (1);
		}
	}
	return (0);
}

int	check_redirect_filename(char **tokens, int *i, t_shell *shell, t_cmd *cmd)
{
	if (!tokens[*i + 1])
	{
		handle_syntax_error("newline", shell);
		cmd->syntax_error = 1;
		return (0);
	}
	if (is_metacharacter(tokens[*i + 1]))
	{
		handle_syntax_error(tokens[*i + 1], shell);
		cmd->syntax_error = 1;
		return (0);
	}
	return (1);
}

int	check_pipe_errors(char **tokens, t_shell *shell, int i)
{
	if (ft_strcmp(tokens[i], "|") == 0)
	{
		if (!tokens[i + 1] || ft_strcmp(tokens[i + 1], "|") == 0)
		{
			handle_syntax_error("|", shell);
			return (1);
		}
	}
	return (0);
}

int	check_syntax_errors(char **tokens, t_shell *shell)
{
	int	i;

	if (!tokens || !tokens[0])
		return (0);
	if (ft_strcmp(tokens[0], "|") == 0)
	{
		handle_syntax_error("|", shell);
		return (1);
	}
	i = 0;
	while (tokens[i])
	{
		if (check_pipe_errors(tokens, shell, i)
			|| check_redir_errors(tokens, shell, &i)
			|| check_quotes(tokens[i], shell))
			return (1);
		i++;
	}
	if (i > 0 && ft_strcmp(tokens[i - 1], "|") == 0)
	{
		handle_syntax_error("|", shell);
		return (1);
	}
	return (0);
}
