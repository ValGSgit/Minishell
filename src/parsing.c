/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:07:29 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 13:21:01 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/**
 * Report a syntax error and set the shell exit status to 2
 */
void	handle_syntax_error(char *token, t_shell *shell)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	shell->exit_status = 2;
}

/**
 * Create a new command node in the pipeline
 */
void	handle_pipeline(t_cmd **current, t_shell *shell)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd_node();
	if (!new_cmd)
	{
		(*current)->syntax_error = 1;
		return ;
	}
	new_cmd->shell = shell;
	(*current)->next = new_cmd;
	*current = new_cmd;
}

static int	is_redirection(char *tokens)
{
	return (ft_strcmp(tokens, ">") == 0 || ft_strcmp(tokens, ">>") == 0
		|| ft_strcmp(tokens, "<") == 0 || ft_strcmp(tokens, "<<") == 0);
}
/**
 * Handle file redirection
 */
void	handle_redirection(t_cmd *cmd, char **tokens, int *i, t_shell *shell)
{
	int		redir_type;
	char	*filename;

	// Determine redirection type
	if (ft_strcmp(tokens[*i], ">") == 0)
		redir_type = REDIR_OUT;
	else if (ft_strcmp(tokens[*i], ">>") == 0)
		redir_type = REDIR_APPEND;
	else if (ft_strcmp(tokens[*i], "<") == 0)
		redir_type = REDIR_IN;
	else if (ft_strcmp(tokens[*i], "<<") == 0)
	{
		// Handle heredoc separately
		if (!tokens[*i + 1])
		{
			handle_syntax_error("newline", shell);
			cmd->syntax_error = 1;
			return ;
		}
		handle_heredoc(cmd, tokens[*i + 1]);
		(*i) += 2; // Skip heredoc delimiter
		return ;
	}
	else
	{
		// Invalid redirection type
		handle_syntax_error(tokens[*i], shell);
		cmd->syntax_error = 1;
		return ;
	}
	// Check if filename is provided
	if (!tokens[*i + 1])
	{
		handle_syntax_error("newline", shell);
		cmd->syntax_error = 1;
		return ;
	}
	// Check if next token is a redirection or pipe
	if (is_metacharacter(tokens[*i + 1]))
	{
		handle_syntax_error(tokens[*i + 1], shell);
		cmd->syntax_error = 1;
		return ;
	}
	// Get filename and create redirection
	filename = tokens[*i + 1];
	create_redir_node(cmd, redir_type, filename);
	(*i) += 2; // Skip redirection type and filename
}

/**
 * Check for syntax errors before parsing
 */
int	check_syntax_errors(char **tokens, t_shell *shell)
{
	int	i;

	// No tokens, no errors
	if (!tokens || !tokens[0])
		return (0);
	// Check for pipe at the beginning
	if (ft_strcmp(tokens[0], "|") == 0)
	{
		handle_syntax_error("|", shell);
		return (1);
	}
	i = 0;
	while (tokens[i])
	{
		// Check for consecutive pipes
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			if (!tokens[i + 1] || ft_strcmp(tokens[i + 1], "|") == 0)
			{
				handle_syntax_error("|", shell);
				return (1);
			}
		}
		// Check for redirection syntax errors
		if (is_redirection(tokens[i]))
		{
			// Missing filename
			if (!tokens[i + 1])
			{
				handle_syntax_error("newline", shell);
				return (1);
			}
			// Check if next token is also a redirection or pipe
			if (is_metacharacter(tokens[i + 1]))
			{
				handle_syntax_error(tokens[i + 1], shell);
				return (1);
			}

			// Check for invalid redirection sequences like "> > file"
			if (tokens[i + 2] && is_redirection(tokens[i + 2]))
			{
				// Allow specific valid cases like "> file > file2"
				if (ft_strcmp(tokens[i + 1], tokens[i + 2]) != 0 && !is_metacharacter(tokens[i + 1]))
				{
					i += 1; // Skip the valid redirection and continue checking
					continue;
				}
			}
		}
		// Check for unclosed quotes
		if (tokens[i][0] == '"' || tokens[i][0] == '\'')
		{
			char quote = tokens[i][0];
			int j = 1;
			while (tokens[i][j] && tokens[i][j] != quote)
				j++;
			if (!tokens[i][j])
			{
				handle_syntax_error("unclosed quote", shell);
				return (1);
			}
		}
		i++;
	}
	// Check for pipe at the end
	if (ft_strcmp(tokens[i - 1], "|") == 0)
	{
		handle_syntax_error("|", shell);
		return (1);
	}
	return (0);
}

/**
 * Main parser function to convert tokens into command structure
 */
t_cmd	*parser(char **tokens, t_shell *shell)
{
	t_cmd *head;
	t_cmd *current;
	int i;

	if (!tokens || !tokens[0])
		return (NULL);

	// Check for syntax errors before parsing
	if (check_syntax_errors(tokens, shell))
		return (NULL);

	head = create_cmd_node();
	if (!head)
		return (NULL);

	current = head;
	current->shell = shell;

	i = 0;
	while (tokens[i])
	{
		// Handle pipes
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			// Check that current command has content
			if (!current->args && !current->redirs)
			{
				handle_syntax_error("|", shell);
				free_cmd(head);
				return (NULL);
			}

			// Special case: pipe followed by redirection (e.g.,"echo | > file")
			if (tokens[i + 1] && is_redirection(tokens[i + 1])
				&& (ft_strcmp(tokens[i + 1], ">") == 0 || ft_strcmp(tokens[i
						+ 1], ">>") == 0))
			{
				handle_pipeline(&current, shell);
				i++;
				continue ;
			}

			handle_pipeline(&current, shell);
			if (current->syntax_error)
			{
				free_cmd(head);
				return (NULL);
			}
			i++;
			continue ;
		}

		// Handle redirections
		if (is_redirection(tokens[i]))
		{
			handle_redirection(current, tokens, &i, shell);
			if (current->syntax_error)
			{
				free_cmd(head);
				return (NULL);
			}
			continue ; // i was incremented in handle_redirection
		}

		// Handle normal arguments
		add_argument(current, tokens[i]);
		i++;
	}

	return (head);
}