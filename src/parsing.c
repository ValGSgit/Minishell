/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:43:36 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/26 15:05:21 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

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
	int	count;

	count = 0;
	while (tokens[count] && !is_metacharacter(tokens[count]))
		count++;
	return (count);
}

/* Handles redirections and adds them to the command structure */
// void	handle_redirections(t_cmd *cmd, char **tokens, int *i)
// {
// 	if (!tokens[*i + 1]) // Check for missing redirection target
// 	{
// 		write(2, "Syntax error: missing redirection target\n", 41);
// 		cmd->syntax_error = true;
// 		return ;
// 	}
// 	if (ft_strcmp(tokens[*i], "<") == 0)
// 		add_redir_node(&cmd->redirs, create_redir_node(REDIR_IN,
// 				tokens[++(*i)]));
// 	else if (ft_strcmp(tokens[*i], ">") == 0)
// 		add_redir_node(&cmd->redirs, create_redir_node(REDIR_OUT,
// 				tokens[++(*i)]));
// 	else if (ft_strcmp(tokens[*i], ">>") == 0)
// 		add_redir_node(&cmd->redirs, create_redir_node(REDIR_APPEND,
// 				tokens[++(*i)]));
// 	else if (ft_strcmp(tokens[*i], "<<") == 0)
// 		add_redir_node(&cmd->redirs, create_redir_node(REDIR_HEREDOC,
// 				tokens[++(*i)]));
// }
void	handle_redirections(t_cmd *cmd, char **tokens, int *i)
{
    //char	*redir = tokens[*i];
    //char	*target;

    // Check if the token contains a redirection operator without a space
    // if (ft_strncmp(redir, "<", 1) == 0 || ft_strncmp(redir, ">", 1) == 0)
    // {
    //     if (ft_strlen(redir) > 1 && (redir[1] == '<' || redir[1] == '>'))
    //         target = &redir[2]; // Handle "<<" or ">>"
    //     else
    //         target = &redir[1]; // Handle "<" or ">"
    //     if (*target) // If there's a target directly after the operator
    //     {
    //         if (ft_strncmp(redir, "<<", 2) == 0)
    //             add_redir_node(&cmd->redirs, create_redir_node(REDIR_HEREDOC, target));
    //         else if (ft_strncmp(redir, ">>", 2) == 0)
    //             add_redir_node(&cmd->redirs, create_redir_node(REDIR_APPEND, target));
    //         else if (redir[0] == '<')
    //             add_redir_node(&cmd->redirs, create_redir_node(REDIR_IN, target));
    //         else if (redir[0] == '>')
    //             add_redir_node(&cmd->redirs, create_redir_node(REDIR_OUT, target));
    //         return;
    //     }
	// }
    // Handle cases where the redirection target is in the next token
    if (!tokens[*i + 1]) // Check for missing redirection target
    {
        write(2, "Syntax error: missing redirection target\n", 41);
        cmd->syntax_error = true;
        return;
    }
    if (ft_strcmp(tokens[*i], "<") == 0)
        add_redir_node(&cmd->redirs, create_redir_node(REDIR_IN, tokens[++(*i)]));
    else if (ft_strcmp(tokens[*i], ">") == 0)
        add_redir_node(&cmd->redirs, create_redir_node(REDIR_OUT, tokens[++(*i)]));
    else if (ft_strcmp(tokens[*i], ">>") == 0)
        add_redir_node(&cmd->redirs, create_redir_node(REDIR_APPEND, tokens[++(*i)]));
    else if (ft_strcmp(tokens[*i], "<<") == 0)
        add_redir_node(&cmd->redirs, create_redir_node(REDIR_HEREDOC, tokens[++(*i)]));
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
		return ;
	}
	// Create a new command node for the pipeline
	pipeline = create_cmd_node();
	if (!pipeline)
	{
		write(2, "Error: failed to allocate memory for pipeline\n", 46);
		(*cmd)->syntax_error = true;
		return ;
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

/* Main parser function */
t_cmd	*parser(char **tokens)
{
	t_cmd	*start_cmd;
	t_cmd	*cmd_to_pass;
	int		i;
	int		argc;

	start_cmd = NULL;
	cmd_to_pass = NULL;
	if (!tokens)
		return (NULL);
	i = -1;
	while (tokens[++i])
	{
		if (!start_cmd && (start_cmd = create_cmd_node()))
			cmd_to_pass = start_cmd;
		else if (!cmd_to_pass)
			break ;
		if (is_metacharacter(tokens[i]))
		{
			if (ft_strcmp(tokens[i], "|") == 0)
				handle_pipeline(&cmd_to_pass);
			else
				handle_redirections(cmd_to_pass, tokens, &i);
		}
		else
		{
			argc = count_args(&tokens[i]);
			cmd_to_pass->args = build_args_array(&tokens[i], argc);
			if (!cmd_to_pass->args)
				return (free_cmd(start_cmd), NULL);
			i += argc - 1;
		}
		if (cmd_to_pass->syntax_error) // Stop parsing on syntax error
			return (free_cmd(start_cmd), NULL);
	}
	if (cmd_to_pass && !cmd_to_pass->args)
		// Check for empty command after the last pipe
	{
		write(2, "Syntax error: empty command after pipe\n", 39);
		return (free_cmd(start_cmd), NULL);
	}
	return (start_cmd);
}
