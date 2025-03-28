/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:43:36 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/28 13:30:07 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

/* Checks if the token is quoted */
bool	is_quoted(char *token)
{
	if (!token)
	{
		return (false);
	}
	return ((token[0] == '\'' && token[ft_strlen(token) - 1] == '\'')
		|| (token[0] == '"' && token[ft_strlen(token) - 1] == '"'));
}

/* Checks if the token is a metacharacter (|, <, >, >>, <<) */
bool	is_metacharacter(char *token)
{
	if (is_quoted(token))
		return (false);
	return (ft_strcmp(token, "|") == 0 || ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0
		|| ft_strcmp(token, "<<") == 0);
}
/* Handles syntax errors for unexpected tokens */
static bool	handle_syntax_error(char *token, t_shell *shell)
{
	if (!token || ft_strcmp(token, "|") == 0)
		write(2, "syntax error near unexpected token `|'\n", 39);
	else if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		write(2, "syntax error near unexpected token `newline'\n", 45);
	else if (ft_strcmp(token, ">>>") == 0)
		write(2, "syntax error near unexpected token `>'\n", 39);
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

/* Adds an argument to the command */
void	add_argument(t_cmd *node, char *arg)
{
	char	**new_args;
	int		i;

	if (!node || !arg)
		return ;
	if (!node->args)
	{
		node->args = ft_calloc(2, sizeof(char *));
		if (!node->args)
			return ;
		node->args[0] = ft_strdup(arg);
		if (!node->args[0])
			return ;
	}
	else
	{
		i = 0;
		while (node->args[i])
			i++;
		new_args = ft_calloc(i + 2, sizeof(char *));
		if (!new_args)
			return ;
		i = -1;
		while (node->args[++i])
		{
			new_args[i] = node->args[i];
			node->args[i] = NULL;
		}
		new_args[i] = ft_strdup(arg);
		if (!new_args[i])
		{
			free_tokens(new_args);
			return ;
		}
		free(node->args);
		node->args = new_args;
	}
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
		handle_syntax_error(tokens[*i + 1] ? tokens[*i + 1] : "newline", shell);
		cmd->syntax_error = true;
		return ;
	}
	if (ft_strcmp(tokens[*i], ">") == 0)
		create_redir_node(cmd, REDIR_OUT, tokens[++(*i)]);
	else if (ft_strcmp(tokens[*i], ">>") == 0)
		create_redir_node(cmd, REDIR_APPEND, tokens[++(*i)]);
	else if (ft_strcmp(tokens[*i], "<") == 0)
		create_redir_node(cmd, REDIR_IN, tokens[++(*i)]);
	else if (ft_strcmp(tokens[*i], "<<") == 0)
		create_redir_node(cmd, REDIR_HEREDOC, tokens[++(*i)]);
	else
	{
		handle_syntax_error(tokens[*i], shell);
		cmd->syntax_error = true;
	}
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
		if (!head && (head = create_cmd_node()))
			current = head;
		else if (!current)
			break ;
		if (is_metacharacter(tokens[i]) && ft_strcmp(tokens[i], "|") == 0)
		{
			if (!current->args && !current->redirs)
			{
				handle_syntax_error("|", shell);
				return (free_cmd(head), NULL);
			}
			handle_pipeline(&current, shell);
		}
		else if (is_metacharacter(tokens[i]))
			handle_redirections(current, tokens, &i, shell);
		else
			add_argument(current, tokens[i]);
	}
	return (head);
}
