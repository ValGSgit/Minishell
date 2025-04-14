/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:13:45 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 11:18:09 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_redirection(t_cmd *cmd, char **tokens, int *i, t_shell *shell)
{
	int		redir_type;
	char	*filename;

	if (ft_strcmp(tokens[*i], ">>") == 0 && same_length(tokens[*i], ">>"))
		redir_type = REDIR_APPEND;
	else if (ft_strcmp(tokens[*i], "<<") == 0 && same_length(tokens[*i], "<<"))
	{
		handle_heredoc_redirect(cmd, tokens, i, shell);
		return ;
	}
	else if (ft_strcmp(tokens[*i], ">") == 0 && same_length(tokens[*i], ">"))
		redir_type = REDIR_OUT;
	else if (ft_strcmp(tokens[*i], "<") == 0 && same_length(tokens[*i], "<"))
		redir_type = REDIR_IN;
	else
	{
		handle_syntax_error(tokens[*i], shell);
		cmd->syntax_error = 1;
		return ;
	}
	if (!check_redirect_filename(tokens, i, shell, cmd))
		return ;
	filename = tokens[*i + 1];
	create_redir_node(cmd, redir_type, filename);
	(*i) += 2;
}

void	handle_pipe_token(t_cmd **current, char **tokens, int *i,
		t_shell *shell)
{
	if (!(*current)->args && !(*current)->redirs)
	{
		handle_syntax_error("|", shell);
		(*current)->syntax_error = 1;
		return ;
	}
	if (tokens[*i + 1] && is_redirection(tokens[*i + 1]) && (ft_strcmp(tokens[*i
					+ 1], ">") == 0 || ft_strcmp(tokens[*i + 1], ">>") == 0))
	{
		handle_pipeline(current, shell);
		(*i)++;
		return ;
	}
	handle_pipeline(current, shell);
	(*i)++;
}

void	handle_parser_token(t_cmd **current, char **tokens, int *i,
		t_shell *shell)
{
	if (ft_strcmp(tokens[*i], "|") == 0)
	{
		handle_pipe_token(current, tokens, i, shell);
	}
	else if (is_redirection(tokens[*i]))
	{
		handle_redirection(*current, tokens, i, shell);
	}
	else
	{
		add_argument(*current, tokens[*i]);
		(*i)++;
	}
}
