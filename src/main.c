/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/27 12:30:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_input(t_shell *shell, char *input)
{
	char	**tokens;
	int		i;
	bool	only_whitespace;

	if (!input)
		return (0);
	i = 0;
	only_whitespace = true;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
		{
			only_whitespace = false;
			break;
		}
		i++;
	}
	if (only_whitespace)
		return (safe_free(input), 0);
	if (*input)
		add_history(input);
	tokens = lexer(input);
	if (!tokens)
	{
		safe_free(input);
		return (0);
	}
	shell->cmd = parser(tokens, shell);
	if (!shell->cmd)
		return (safe_free(input), free_tokens(tokens), 0);
	expand_nodes(shell->cmd, shell);
	//debug_shell_state(tokens, shell->cmd, "tes");
	if (shell->cmd->args || shell->cmd->redirs)
		executor(shell->cmd, shell);
	free_cmd(shell->cmd);
	shell->cmd = NULL;
	free_tokens(tokens);
	safe_free(input);
	return (0);
}

void	minishell_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;

	shell->cmd = NULL;
	prompt = xmalloc(ft_strlen("Minishell-> ") + 1);
	if (prompt)
		ft_strcpy(prompt, "Minishell-> ");
	while (1)
	{
		setup_signals();
		input = readline(prompt);
		if (!input)
		{
			if (shell->is_interactive)
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			safe_free(prompt);
			if (g_signal_received == 130)
				shell->exit_status = 130;
			break ;
		}
		if (handle_input(shell, input) == 1)
		{
			safe_free(prompt);
			break ;
		}
		if (g_signal_received == 130)
		{
			shell->exit_status = 130;
			g_signal_received = 0;
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
			safe_free(pwd_var);
			safe_free(cwd);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
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
	rl_clear_history();
	cleanup_shell(&shell);
	return (shell.exit_status);
}
