/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:43:36 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/03 13:23:50 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Handles syntax errors for unexpected tokens */
static bool	handle_syntax_error(char *token, t_shell *shell)
{
	if (!token || ft_strcmp(token, "|") == 0)
		write(2, "syntax error near unexpected token `|'\n", 40);
	else if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		write(2, " syntax error near unexpected token `newline'\n", 46);
	else if (ft_strcmp(token, ">>>") == 0)
		write(2, "syntax error near unexpected token `>'\n", 39);
	else if (ft_strcmp(token, ">") == 0 && ft_strcmp(token + 1, ">") == 0)
		write(2, " syntax error near unexpected token `newline'\n", 46);
	else
	{
		write(2, "syntax error near unexpected token `", 37);
		write(2, token, ft_strlen(token));
		write(2, "'\n", 2);
	}
	shell->exit_status = 2;
	return (true);
}

/* Handles pipelines */
void	handle_pipeline(t_cmd **current, t_shell *shell)
{
	t_cmd	*new_cmd;

	if (!(*current)->args && !(*current)->redirs)
	{
		handle_syntax_error("|", shell);
		(*current)->syntax_error = true;
		shell->exit_status = 2;
		return ;
	}
	new_cmd = create_cmd_node();
	if (!new_cmd)
	{
		perror("minishell");
		shell->exit_status = 1;
		return ;
	}
	(*current)->next = new_cmd;
	*current = new_cmd;
}

/* Builds the arguments array for a command */
char	**build_args_array(char **tokens, int argc)
{
	char	**args;
	int		i;

	if (!tokens || argc <= 0)
		return (NULL);
	args = ft_calloc(argc + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (i < argc && tokens[i])
	{
		args[i] = ft_strdup(tokens[i]);
		if (!args[i])
		{
			free_tokens(args);
			return (NULL);
		}
		i++;
	}
	return (args);
}

/* Handles redirections */
void	handle_redirections(t_cmd *cmd, char **tokens, int *i, t_shell *shell)
{
	if (is_quoted(tokens[*i]))
	{
		add_argument(cmd, tokens[*i]);
		return ;
	}
	if (!tokens[*i + 1] || is_metacharacter(tokens[*i + 1]))
	{
		if (tokens[*i + 1])
			handle_syntax_error(tokens[*i], shell);
		else
			handle_syntax_error(tokens[*i], shell);
		cmd->syntax_error = true;
		return ;
	}
	if (ft_strncmp(tokens[*i], ">>", 2) == 0)
		create_redir_node(cmd, REDIR_APPEND, tokens[++(*i)]);
	else if (ft_strncmp(tokens[*i], "<<", 2) == 0)
		handle_heredoc(cmd, tokens[++(*i)]);
	else if (ft_strncmp(tokens[*i], ">", 1) == 0)
		create_redir_node(cmd, REDIR_OUT, tokens[++(*i)]);
	else if (ft_strncmp(tokens[*i], "<", 1) == 0)
		create_redir_node(cmd, REDIR_IN, tokens[++(*i)]);
	else
		cmd->syntax_error = handle_syntax_error(tokens[++(*i)], shell);
}

/* Main parser function */
t_cmd	*parser(char **tokens, t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*current;
	int		i;

	head = NULL;
	current = NULL;
	i = -1;
	if (!tokens)
		return (NULL);
	while (tokens[++i])
	{
		if (!head)
		{
			head = create_cmd_node();
			current = head;
		}
		else if (!current)
			break ;
		current->shell = shell;
		if (is_metacharacter(tokens[i]) && ft_strcmp(tokens[i], "|") == 0)
		{
			if (!current->args && !current->redirs)
				return (handle_syntax_error("|", shell), free_cmd(head), NULL);
			handle_pipeline(&current, shell);
		}
		else if (is_metacharacter(tokens[i]))
		{
			handle_redirections(current, tokens, &i, shell);
			if (current->syntax_error)
				return (free_cmd(head), NULL);
		}
		else
			add_argument(current, tokens[i]);
	}
	return (head);
}
