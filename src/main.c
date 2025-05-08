/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/07 10:30:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern volatile sig_atomic_t	g_signal_received;

void	handle_signal_exit(t_shell *shell)
{
	if (shell->is_interactive)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (g_signal_received == SIGINT)
		shell->exit_status = 128 + SIGINT;
	close(0);
	close(1);
}

void	process_received_signal(t_shell *shell)
{
	if (g_signal_received == SIGINT)
	{
		shell->exit_status = 130;
		g_signal_received = 0;
	}
	else if (g_signal_received)
	{
		shell->exit_status = 130;
		g_signal_received = 0;
	}
}

int	process_shell_input(t_shell *shell, char *input)
{
	process_received_signal(shell);
	if (!input)
	{
		handle_signal_exit(shell);
		return (1);
	}
	if (handle_input(shell, input) == 1)
		return (1);
	return (0);
}

void	minishell_loop(t_shell *shell)
{
	char		*input;

	shell->cmd = NULL;
	while (1)
	{
		setup_signals();
		cleanup_heredocs(shell);
		input = readline(PROMPT);
		if (!input)
		{
			handle_signal_exit(shell);
			break ;
		}
		if (unclosed_check(input))
		{
			print_error_message("minishell: quotes not closed: ", input, "'\n");
			shell->exit_status = 2;
			add_history(input);
			free(input);
			continue ;
		}
		if (process_shell_input(shell, input))
			break ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	g_signal_received = 0;
	ft_memset(&shell, 0, sizeof(t_shell));
	if (*envp)
		shell.env = copy_env(envp);
	else
	{
		shell.env = malloc(sizeof(char **));
		shell.env[0] = NULL;
	}
	initialize_shell(&shell, argv);
	shell.path_unset = false;
	shell.exit_status = 0;
	shell.is_interactive = isatty(STDIN_FILENO);
	shell.signal_status = 0;
	minishell_loop(&shell);
	cleanup_heredocs(&shell);
	rl_clear_history();
	cleanup_shell(&shell);
	return (shell.exit_status);
}
