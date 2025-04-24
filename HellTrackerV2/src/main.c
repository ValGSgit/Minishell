/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 18:15:32 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// debug_shell_state(tokens, NULL, "Before parsing");
// debug_shell_state(tokens, shell->cmd, "After parsing");
// debug_shell_state(tokens, shell->cmd, "After expansion");
// if (shell->cmd && shell->cmd->args && shell->cmd->args[0] != NULL)

int	handle_input(t_shell *shell, char *input)
{
	char	**tokens;

	if (!input)
		return (0);
	if (*input)
		add_history(input);
	tokens = lexer(input);
	if (!tokens)
	{
		xfree(input);
		return (0);
	}
	shell->cmd = parser(tokens, shell);
	if (!shell->cmd)
	{
		xfree(input);
		free_tokens(tokens);
		return (0);
	}
	expand_nodes(shell->cmd, shell);
	if (shell->cmd->args || shell->cmd->redirs)
	{
		executor(shell->cmd, shell);
	}
	free_cmd(shell->cmd);
	shell->cmd = NULL;  /* Set to NULL to avoid dangling pointer */
	free_tokens(tokens);  /* Use free_tokens instead of manual freeing */
	xfree(input);
	return (0);
}

void	minishell_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;

	shell->cmd = NULL;
	prompt = ft_strdup("Minishell-> ");
	while (1)
	{
		setup_signals();
		// prompt = update_prompt();
		// if (!prompt)
		// 	prompt = ft_strdup("Minishell-> ");
		input = readline(prompt);
		// xfree(prompt);
		if (!input)
		{
			if (shell->is_interactive)
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			break;
		}
		if (handle_input(shell, input) == 1)
		{
			xfree(input);
			break ;
		}
	}
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
			pwd_var = ft_strjoin("PWD=", cwd);
			if (pwd_var)
				update_or_add_env(pwd_var, &shell->env);
			xfree(pwd_var);
			xfree(cwd);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	if (*envp)
		shell.env = copy_env(envp);
	if (!shell.env)
	{
		write(2, "environment copy failed\n", 25);
		exit(EXIT_FAILURE);
	}
	initialize_shell(&shell, argv);
	shell.path_unset = false;
	shell.exit_status = 0;
	shell.is_interactive = isatty(STDIN_FILENO);
	shell.signal_status = 0;
	minishell_loop(&shell);
	rl_clear_history();
	free_env(shell.env);
	cleanup_temp_files();  /* Clean up any temporary heredoc files */
	print_memory_leaks(); // Print any memory leaks before exiting
	clean_memory(); // Clean up any remaining tracked memory
	return (shell.exit_status);
}
