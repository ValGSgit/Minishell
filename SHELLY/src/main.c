/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/27 12:30:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern volatile sig_atomic_t	g_signal_received;

void	handle_signal_exit(t_shell *shell)
{
	if (shell->is_interactive)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (g_signal_received == 130)
		shell->exit_status = 130;
}

int	process_shell_input(t_shell *shell, char *input, char *prompt)
{
	if (!input)
	{
		handle_signal_exit(shell);
		return (1);
	}
	if (handle_input(shell, input) == 1)
	{
		safe_free(prompt);
		return (1);
	}
	if (g_signal_received == 130)
	{
		shell->exit_status = 130;
		g_signal_received = 0;
	}
	return (0);
}

void	minishell_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;

	shell->cmd = NULL;
	prompt = ft_strdup("Minishell-> ");
	if (!prompt)
		prompt = ft_strdup("minishell-> ");
	while (1)
	{
		setup_signals();
		input = readline(prompt);
		if (process_shell_input(shell, input, prompt))
			break ;
	}
	safe_free(prompt);
}

void	initialize_shell(t_shell *shell, char **argv)
{
	char	*prog_name;
	char	*cwd;
	char	*pwd_var;

	prog_name = ft_strrchr(argv[0], '/');
	if (prog_name)
		prog_name++;
	else
		prog_name = argv[0];
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

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	ft_memset(&shell, 0, sizeof(t_shell));
	if (*envp)
		shell.env = copy_env(envp);
	else
		shell.env = NULL;
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
