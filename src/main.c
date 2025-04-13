/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/13 17:49:33 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_signal_received = 0;


int handle_input(t_shell *shell, char *input)
{
    char **tokens;
    
    if (!input)
        return (0);
    if (*input)
        add_history(input);
    tokens = lexer(input);
    //debug_shell_state(tokens, NULL, "Before parsing");
    if (!tokens)
    {
        free(input);
        return 0;
    }
    shell->cmd = parser(tokens, shell);
    //debug_shell_state(tokens, shell->cmd, "After parsing");
    if (!shell->cmd)
        return (free(input), free_tokens(tokens), 0);
    expand_nodes(shell->cmd, shell);
    //debug_shell_state(tokens, shell->cmd, "After expansion");
    // Add check to prevent segfault
    //if (shell->cmd && shell->cmd->args && shell->cmd->args[0] != NULL)
    executor(shell->cmd, shell);   
    free_cmd(shell->cmd);
    if (tokens)
        free_tokens(tokens);
    free(input);
    return 0;
}

void minishell_loop(t_shell *shell)
{
    char    *input;
    char    *prompt;

    shell->cmd = NULL;
    prompt = "Minishell-> ";
    while (1)
    {
        setup_signals();
        input = readline(prompt);
        
        // Handle Ctrl+D (EOF)
        if (!input)
        {
            // Only print "exit" in interactive mode
            if (shell->is_interactive)
               ft_putstr_fd("exit\n", STDOUT_FILENO);
            // Break the loop to exit the shell
            break;
        }
        // Process the input normally
        if (handle_input(shell, input) == 1)
        {
            free(input);
            break;
        }
    }
}

void	initialize_shell(t_shell *shell, char **envp, char **argv)
{
	char	*prog_name;
	char	*cwd;
	char	*pwd_var;

	if (*envp)
		shell->env = copy_env(envp);
	if (!shell->env)
	{
		write(2, "environment copy failed\n", 25);
		exit(EXIT_FAILURE);
	}
	prog_name = ft_strrchr(argv[0], '/');
	if (prog_name)
		prog_name++;
	else
		prog_name = argv[0];
	if (ft_strcmp(prog_name, "minishell") == 0 || 
		ft_strcmp(prog_name, "./minishell") == 0 || 
		ft_strcmp(prog_name, "bash") == 0)
		update_shlvl(shell);
	if (!get_env_value("PWD", shell->env))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			pwd_var = ft_strjoin("PWD=", cwd);
			if (pwd_var)
				update_or_add_env(pwd_var, &shell->env);
			(free(pwd_var), free(cwd));
		}
	}
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;

	(void)argc;
	initialize_shell(&shell, envp, argv);
	shell.exit_status = 0;
	shell.is_interactive = isatty(STDIN_FILENO);
	shell.signal_status = 0;
	minishell_loop(&shell);
	rl_clear_history();
	free_env(shell.env);
	return (shell.exit_status);
}
