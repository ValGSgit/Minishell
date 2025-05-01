/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:33:20 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 20:49:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_tokens(char **tokens)
{
	int	i;

	if (!tokens)
	{
		printf("Tokens: NULL\n");
		return ;
	}
	printf("Tokens:\n");
	i = 0;
	while (tokens[i])
	{
		printf("  [%d]: %s\n", i, tokens[i]);
		i++;
	}
}

static void	print_redirections(t_redir *redir)
{
	printf("    Redirections:\n");
	while (redir)
	{
		printf("      - Type: %d, File: %s\n", redir->type, redir->file);
		redir = redir->next;
	}
}

static void	print_command_args(t_cmd *current)
{
	int	i;

	printf("  Command: %s\n", current->args[0]);
	printf("    Arguments:\n");
	i = 1;
	while (current->args[i])
	{
		printf("      - %s\n", current->args[i]);
		i++;
	}
}

void	print_parsed_commands(t_cmd *cmd)
{
	t_cmd	*current;

	if (!cmd)
	{
		printf("Parsed Commands: NULL\n");
		return ;
	}
	printf("Parsed Commands:\n");
	current = cmd;
	while (current)
	{
		if (current->args)
			print_command_args(current);
		if (current->redirs)
			print_redirections(current->redirs);
		current = current->next;
	}
}

void	debug_shell_state(char **tokens, t_cmd *cmd, const char *stage)
{
	printf("\n=== Debug: %s ===\n", stage);
	if (tokens)
		print_tokens(tokens);
	if (cmd)
		print_parsed_commands(cmd);
	printf("====================\n");
}