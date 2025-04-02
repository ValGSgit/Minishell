/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:33:20 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/01 12:09:41 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/* Print tokens generated by the lexer */
void	print_tokens(char **tokens)
{
    int	i;

    if (!tokens)
    {
        printf("Tokens: NULL\n");
        return;
    }
    printf("Tokens:\n");
    i = 0;
    while (tokens[i])
    {
        printf("  [%d]: %s\n", i, tokens[i]);
        i++;
    }
}

/* Print the parsed command structure */
void	print_parsed_commands(t_cmd *cmd)
{
    t_cmd	*current;
    int		i;

    if (!cmd)
    {
        printf("Parsed Commands: NULL\n");
        return;
    }
    printf("Parsed Commands:\n");
    current = cmd;
    while (current)
    {
        printf("  Command: %s\n", current->args[0]);
        if (current->args)
        {
            printf("    Arguments:\n");
            i = 1;
            while (current->args[i])
            {
                printf("      - %s\n", current->args[i]);
                i++;
            }
        }
        if (current->redirs)
        {
            t_redir *redir = current->redirs;
            printf("    Redirections:\n");
            while (redir)
            {
                printf("      - Type: %d, File: %s\n", redir->type, redir->file);
                redir = redir->next;
            }
        }
        current = current->next;
    }
}

/* Debug function to print the state after each stage */
void	debug_shell_state(char **tokens, t_cmd *cmd, const char *stage)
{
    printf("\n=== Debug: %s ===\n", stage);
    if (tokens)
        print_tokens(tokens);
    if (cmd)
        print_parsed_commands(cmd);
    printf("====================\n");
}