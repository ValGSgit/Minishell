/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:43:36 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/24 11:44:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

// char	**build_args_array(char **tokens, int argc)
// {
// 	char	**args;
// 	int		i;

// 	args = ft_calloc(argc + 1, sizeof(char *));
// 	if (!args)
// 		return (NULL);
// 	i = -1;
// 	while (++i < argc && tokens[i])
// 	{
// 		args[i] = ft_strdup(tokens[i]);
// 		if (args[i] == NULL)
// 		{
// 			free_tokens(args);
// 			return (NULL);
// 		}
// 	}
// 	return (args);
// }

// bool	is_metacharacter(char *token)
// {
// 	return (ft_strcmp(token, "|") == 0 || ft_strcmp(token, "<") == 0
// 		|| ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0
// 		|| ft_strcmp(token, "<<") == 0);
// }

// int	count_args(char **tokens)
// {
// 	int	count;

// 	count = 0;
// 	while (tokens[count] && !is_metacharacter(tokens[count]))
// 		count++;
// 	return (count);
// }

// void	handle_redirections(t_cmd *cmd, char **tokens, int *i)
// {
// 	if (ft_strcmp(tokens[*i], "<") == 0)
// 		add_redir_node(&cmd->input, create_redir_node(REDIR_IN,
// 				tokens[++(*i)]));
// 	else if (ft_strcmp(tokens[*i], ">") == 0)
// 		add_redir_node(&cmd->output, create_redir_node(REDIR_OUT,
// 				tokens[++(*i)]));
// 	else if (ft_strcmp(tokens[*i], ">>") == 0)
// 		add_redir_node(&cmd->output, create_redir_node(REDIR_APPEND,
// 				tokens[++(*i)]));
// 	else if (ft_strcmp(tokens[*i], "<<") == 0)
// 		add_redir_node(&cmd->input, create_redir_node(REDIR_HEREDOC,
// 				tokens[++(*i)]));
// }

// void	handle_pipeline(t_cmd **cmd, char **tokens, int i)
// {
// 	t_cmd	*pipeline;

// 	if (ft_strcmp(tokens[i], "|") == 0)
// 	{
// 		pipeline = create_cmd_node();
// 		if (pipeline)
// 		{
// 			(*cmd)->next = pipeline;
// 			*cmd = pipeline;
// 		}
// 	}
// }

// int	is_redirection(char *token)
// {
// 	return (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
// 		|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0);
// }

// t_cmd	*parser(char **tokens)
// {
// 	t_cmd *head;
// 	t_cmd *current;
// 	int i;
// 	int argc;

// 	head = NULL;
// 	current = NULL;
// 	if (!tokens)
// 		return (NULL);
// 	i = -1;
// 	while (tokens[++i])
// 	{
// 		if (!head && (head = create_cmd_node()))
// 			current = head;
// 		else if (!current)
// 			break ;
// 		if (is_redirection(tokens[i]))
// 			handle_redirections(current, tokens, &i);
// 		else if (ft_strcmp(tokens[i], "|") == 0)
// 			handle_pipeline(&current, tokens, i);
// 		else
// 		{
// 			argc = count_args(&tokens[i]);
// 			current->args = build_args_array(&tokens[i], argc);
// 			i += argc - 1;
// 		}
// 	}
// 	return (head);
// }


/* Checks if the token is a metacharacter (|, <, >, >>, <<) */
bool	is_metacharacter(char *token)
{
    return (ft_strcmp(token, "|") == 0 || ft_strcmp(token, "<") == 0
        || ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0
        || ft_strcmp(token, "<<") == 0);
}

/* Counts the number of arguments until the next metacharacter */
int	count_args(char **tokens)
{
    int	count = 0;

    while (tokens[count] && !is_metacharacter(tokens[count]))
        count++;
    return (count);
}

/* Handles redirections and adds them to the command structure */
void	handle_redirections(t_cmd *cmd, char **tokens, int *i)
{
    if (!tokens[*i + 1]) // Check for missing redirection target
    {
		write(2, "Syntax error: missing redirection target\n", 41);
        cmd->syntax_error = true;
        return;
    }
    if (ft_strcmp(tokens[*i], "<") == 0)
        add_redir_node(&cmd->input, create_redir_node(REDIR_IN, tokens[++(*i)]));
    else if (ft_strcmp(tokens[*i], ">") == 0)
        add_redir_node(&cmd->output, create_redir_node(REDIR_OUT, tokens[++(*i)]));
    else if (ft_strcmp(tokens[*i], ">>") == 0)
        add_redir_node(&cmd->output, create_redir_node(REDIR_APPEND, tokens[++(*i)]));
    else if (ft_strcmp(tokens[*i], "<<") == 0)
        add_redir_node(&cmd->input, create_redir_node(REDIR_HEREDOC, tokens[++(*i)]));
}

/* Handles pipelines and links commands together */
void	handle_pipeline(t_cmd **cmd)
{
    t_cmd	*pipeline;

    // Check for empty command before the pipe
    if (!(*cmd)->args)
    {
        write(2, "Syntax error: empty command before pipe\n", 39);
        (*cmd)->syntax_error = true;
        return;
    }
    // Create a new command node for the pipeline
    pipeline = create_cmd_node();
    if (!pipeline)
    {
        write(2, "Error: failed to allocate memory for pipeline\n", 46);
        (*cmd)->syntax_error = true;
        return;
    }
    // Link the current command to the new pipeline node
    (*cmd)->next = pipeline;
    *cmd = pipeline;
}

/* Builds the arguments array for a command */
char	**build_args_array(char **tokens, int argc)
{
    char	**args;
    int		i;

    args = ft_calloc(argc + 1, sizeof(char *));
    if (!args)
        return (NULL);
    i = -1;
    while (++i < argc && tokens[i])
    {
        args[i] = ft_strdup(tokens[i]);
        if (!args[i])
        {
            free_tokens(args);
            return (NULL);
        }
    }
    return (args);
}

/* Main parser function */
t_cmd	*parser(char **tokens)
{
    t_cmd	*head;
    t_cmd	*current;
    int		i;
    int		argc;

    head = NULL;
    current = NULL;
    if (!tokens)
        return (NULL);
    i = -1;
    while (tokens[++i])
    {
        if (!head && (head = create_cmd_node()))
            current = head;
        else if (!current)
            break;
        if (is_metacharacter(tokens[i]))
        {
            if (ft_strcmp(tokens[i], "|") == 0)
                handle_pipeline(&current);
            else
                handle_redirections(current, tokens, &i);
        }
        else
        {
            argc = count_args(&tokens[i]);
            current->args = build_args_array(&tokens[i], argc);
            if (!current->args)
                return (free_cmd(head), NULL);
            i += argc - 1;
        }
        if (current->syntax_error) // Stop parsing on syntax error
            return (free_cmd(head), NULL);
    }
    if (current && !current->args) // Check for empty command after the last pipe
    {
		write(2, "Syntax error: empty command after pipe\n", 39);
        return (free_cmd(head), NULL);
    }
    return (head);
}
