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
	shell->exit_status = 128 + SIGINT;
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
		shell->exit_status = 128 + g_signal_received;
		g_signal_received = 0;
	}
}

int	process_shell_input(t_shell *shell, char *input)
{
	if (!input)
	{
		process_received_signal(shell);
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

void	initialize_shell_env(t_shell *shell, char *prog_name)
{
	char	*cwd;
	char	*pwd_var;

	if (ft_strcmp(prog_name, "minishell") == 0 || ft_strcmp(prog_name,
			"./minishell") == 0 || ft_strcmp(prog_name, "bash") == 0)
		update_shlvl(shell);
	if (!get_env_value("PWD", shell->env))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			pwd_var = safe_strjoin("PWD=", cwd, 0);
			if (pwd_var)
				update_or_add_env(pwd_var, &shell->env);
			safe_free(pwd_var);
			safe_free(cwd);
		}
	}
}

void	initialize_shell(t_shell *shell, char **argv)
{
	char	*prog_name;

	prog_name = ft_strrchr(argv[0], '/');
	if (prog_name)
		prog_name++;
	else
		prog_name = argv[0];
	initialize_shell_env(shell, prog_name);
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
