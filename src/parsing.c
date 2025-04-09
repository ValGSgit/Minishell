/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:43:36 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 10:56:35 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static bool	handle_syntax_error(char *token, t_shell *shell)
{
	if (!token)
	{
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		shell->exit_status = 2;
		return (true);
	}
	else if (ft_strcmp(token, "|") == 0)
	{
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		shell->exit_status = 2;
		return (true);
	}
	else if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0)
	{
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		shell->exit_status = 2;
		return (true);
	}
	else
	{
		ft_putstr_fd("syntax error near unexpected token `", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd("'\n", 2);
		shell->exit_status = 2;
		return (true);
	}
}

static void	handle_file_creation(char *redir_type, char *file_name)
{
	int	flags;
	int	fd;

	// Set flags based on redirection type
	flags = O_WRONLY | O_CREAT;
	if (ft_strcmp(redir_type, ">") == 0)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	// Create the file
	fd = open(file_name, flags, 0644);
	if (fd >= 0)
		close(fd);
}

void	handle_redirection(t_cmd *cmd, char **tokens, int *i, t_shell *shell)
{
	char	*redir_type;
	char	*file_name;

	redir_type = tokens[*i];
	// Handle case where there's a redirection without a command (bash creates a file)
	if (!cmd->args && (ft_strcmp(redir_type, ">") == 0 || ft_strcmp(redir_type,
				">>") == 0) && tokens[*i + 1])
	{
		file_name = tokens[*i + 1];
		handle_file_creation(redir_type, file_name);
		(*i) += 2; // Skip the redirection and filename
		return ;
	}
	// Handle quoted token
	if (is_quoted(tokens[*i]))
	{
		add_argument(cmd, tokens[*i]);
		(*i)++;
		return ;
	}
	// Handle syntax error cases
	if (!tokens[*i + 1] || is_metacharacter(tokens[*i + 1]))
	{
		cmd->syntax_error = handle_syntax_error(tokens[*i + 1] ? tokens[*i
				+ 1] : NULL, shell);
		(*i)++;
		return ;
	}
	// Normal redirection handling
	if (ft_strncmp(tokens[*i], ">>", 2) == 0)
	{
		create_redir_node(cmd, REDIR_APPEND, tokens[*i + 1]);
		(*i) += 2;
	}
	else if (ft_strncmp(tokens[*i], "<<", 2) == 0)
	{
		handle_heredoc(cmd, tokens[*i + 1]);
		(*i) += 2;
	}
	else if (ft_strncmp(tokens[*i], ">", 1) == 0)
	{
		create_redir_node(cmd, REDIR_OUT, tokens[*i + 1]);
		(*i) += 2;
	}
	else if (ft_strncmp(tokens[*i], "<", 1) == 0)
	{
		create_redir_node(cmd, REDIR_IN, tokens[*i + 1]);
		(*i) += 2;
	}
	else
	{
		cmd->syntax_error = handle_syntax_error(tokens[*i], shell);
		(*i)++;
	}
}

// /* Handles pipelines */
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
			handle_redirection(current, tokens, &i, shell);
			if (current->syntax_error)
				return (free_cmd(head), NULL);
		}
		else
			add_argument(current, tokens[i]);
	}
	return (head);
}
