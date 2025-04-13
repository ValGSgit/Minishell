/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:07:29 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/13 16:37:59 by vagarcia         ###   ########.fr       */
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
/**
 * Handle file redirection
 */
int	same_length(char *tok1, char *tok2)
{
	int len1;
	int len2;

	len1 = ft_strlen(tok1);
	len2 = ft_strlen(tok2);
	if (len1 == len2)
		return (1);
	return (0);
}
 
void	handle_heredoc_redirect(t_cmd *cmd, char **tokens, int *i, t_shell *shell)
{
	if (!tokens[*i + 1])
    {
		handle_syntax_error("newline", shell);
        cmd->syntax_error = 1;
        return;
    }
    if (is_metacharacter(tokens[*i + 1]))
    {
		handle_syntax_error(tokens[*i + 1], shell);
        cmd->syntax_error = 1;
        return;
    }
	handle_heredoc(cmd, tokens[*i + 1]);
    (*i) += 2;
}

void	handle_redirection(t_cmd *cmd, char **tokens, int *i, t_shell *shell)
{
    int		redir_type;
    char	*filename;

    if (ft_strcmp(tokens[*i], ">>") == 0 && same_length(tokens[*i], ">>"))
        redir_type = REDIR_APPEND;
    else if (ft_strcmp(tokens[*i], "<<") == 0 && same_length(tokens[*i], "<<"))
    {
        handle_heredoc_redirect(cmd, tokens, i, shell);
        return;
    }
    else if (ft_strcmp(tokens[*i], ">") == 0 && same_length(tokens[*i], ">")) 
        redir_type = REDIR_OUT;
    else if (ft_strcmp(tokens[*i], "<") == 0 && same_length(tokens[*i], "<"))
        redir_type = REDIR_IN;
    else
    {
        handle_syntax_error(tokens[*i], shell);
        cmd->syntax_error = 1;
        return;
    }
    if (!check_redirect_filename(tokens, i, shell, cmd))
        return;
    filename = tokens[*i + 1];
    create_redir_node(cmd, redir_type, filename);
    (*i) += 2;
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
        if (check_pipe_errors(tokens, shell, i))
            return (1);
        if (check_redir_errors(tokens, shell, &i))
            return (1);
        if (check_quotes(tokens[i], shell))
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


void	handle_pipe_token(t_cmd **current, char **tokens, int *i, t_shell *shell)
{
	if (!(*current)->args && !(*current)->redirs)
	{
		handle_syntax_error("|", shell);
		(*current)->syntax_error = 1;
		return;
	}
	if (tokens[*i + 1] && is_redirection(tokens[*i + 1])
		&& (ft_strcmp(tokens[*i + 1], ">") == 0 
		|| ft_strcmp(tokens[*i + 1], ">>") == 0))
	{
		handle_pipeline(current, shell);
		(*i)++;
		return;
	}
	handle_pipeline(current, shell);
	(*i)++;
}

void	handle_parser_token(t_cmd **current, char **tokens, int *i, t_shell *shell)
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


t_cmd	*parser(char **tokens, t_shell *shell)
{
    t_cmd	*head;
    t_cmd	*current;
    int		i;

    if (!tokens || !tokens[0] || check_syntax_errors(tokens, shell))
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

/**
 * Main parser function to convert tokens into command structure
//  */
// t_cmd	*parser(char **tokens, t_shell *shell)
// {
// 	t_cmd *head;
// 	t_cmd *current;
// 	int i;

// 	if (!tokens || !tokens[0])
// 		return (NULL);

// 	// Check for syntax errors before parsing
// 	if (check_syntax_errors(tokens, shell))
// 		return (NULL);

// 	head = create_cmd_node();
// 	if (!head)
// 		return (NULL);

// 	current = head;
// 	current->shell = shell;

// 	i = 0;
// 	while (tokens[i])
// 	{
// 		// Handle pipes
// 		if (ft_strcmp(tokens[i], "|") == 0)
// 		{
// 			// Check that current command has content
// 			if (!current->args && !current->redirs)
// 			{
// 				handle_syntax_error("|", shell);
// 				free_cmd(head);
// 				return (NULL);
// 			}

// 			// Special case: pipe followed by redirection (e.g.,"echo | > file")
// 			if (tokens[i + 1] && is_redirection(tokens[i + 1])
// 				&& (ft_strcmp(tokens[i + 1], ">") == 0 || ft_strcmp(tokens[i
// 						+ 1], ">>") == 0))
// 			{
// 				handle_pipeline(&current, shell);
// 				i++;
// 				continue ;
// 			}

// 			handle_pipeline(&current, shell);
// 			if (current->syntax_error)
// 			{
// 				free_cmd(head);
// 				return (NULL);
// 			}
// 			i++;
// 			continue ;
// 		}

// 		// Handle redirections
// 		if (is_redirection(tokens[i]))
// 		{
// 			handle_redirection(current, tokens, &i, shell);
// 			if (current->syntax_error)
// 			{
// 				free_cmd(head);
// 				return (NULL);
// 			}
// 			continue ; // i was incremented in handle_redirection
// 		}

// 		// Handle normal arguments
// 		add_argument(current, tokens[i]);
// 		i++;
// 	}

// 	return (head);
// }