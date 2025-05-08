/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:07:15 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 21:07:54 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_redirection(char *tokens)
{
	return (ft_strcmp(tokens, ">") == 0 || ft_strcmp(tokens, ">>") == 0
		|| ft_strcmp(tokens, "<") == 0 || ft_strcmp(tokens, "<<") == 0);
}

int	same_length(char *tok1, char *tok2)
{
	int	len1;
	int	len2;

	len1 = ft_strlen(tok1);
	len2 = ft_strlen(tok2);
	if (len1 == len2)
		return (1);
	return (0);
}

void	handle_heredoc_direct(t_cmd *cmd, char **tokens, int *i,
		t_shell *shell)
{
	if (!tokens[*i + 1])
	{
		handle_syntax_error("newline", shell);
		cmd->syntax_error = 1;
		return ;
	}
	if (is_metacharacter(tokens[*i + 1])
		|| contains_metacharacter(tokens[*i + 1]))
	{
		handle_syntax_error(tokens[*i + 1], shell);
		cmd->syntax_error = 1;
		return ;
	}
	handle_heredoc(cmd, tokens[*i + 1]);
	(*i) += 2;
}

t_cmd	*parser(char **tokens, t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*current;
	int		i;

	if (!tokens || !tokens[0])
		return (NULL);
	head = create_cmd_node();
	if (!head)
		return (NULL);
	current = head;
	current->shell = shell;
	i = 0;
	while (tokens[i] && !current->syntax_error)
	{
		handle_parser_token(&current, tokens, &i, shell);
		if (current->syntax_error)
		{
			free_cmd(head);
			return (NULL);
		}
	}
	return (head);
}
