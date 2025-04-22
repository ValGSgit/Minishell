/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:56 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 12:39:32 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_cmd_info(t_cmd *cmd)
{
	int	i;

	ft_putstr_fd("\nCommand node details:\n", 1);
	if (cmd->args && cmd->args[0])
	{
		ft_putstr_fd("Command: ", 1);
		i = 0;
		while (cmd->args[i])
		{
			ft_putstr_fd(cmd->args[i], 1);
			ft_putstr_fd(" ", 1);
			i++;
		}
		ft_putstr_fd("\n", 1);
	}
	else
		ft_putstr_fd("Command: (empty)\n", 1);
	ft_putstr_fd("Input file: ", 1);
	ft_putstr_fd(cmd->in_file ? cmd->in_file : "(none)", 1);
	ft_putstr_fd("\nOutput file: ", 1);
	ft_putstr_fd(cmd->out_file ? cmd->out_file : "(none)", 1);
	ft_putstr_fd("\n", 1);
}

static void	print_tokens(char **tokens)
{
	int	i;

	ft_putstr_fd("Tokens:\n", 1);
	i = 0;
	while (tokens && tokens[i])
	{
		ft_putstr_fd("[", 1);
		ft_putstr_fd(tokens[i], 1);
		ft_putstr_fd("] ", 1);
		i++;
	}
	ft_putstr_fd("\n\n", 1);
}

void	debug_shell_state(char **tokens, t_cmd *cmd, char *stage)
{
	t_cmd	*current;

	ft_putstr_fd("\n--- DEBUG: ", 1);
	ft_putstr_fd(stage, 1);
	ft_putstr_fd(" ---\n", 1);
	if (tokens)
		print_tokens(tokens);
	ft_putstr_fd("Command structure:\n", 1);
	current = cmd;
	while (current)
	{
		print_cmd_info(current);
		current = current->next;
		if (current)
			ft_putstr_fd("---> Next command\n", 1);
	}
}