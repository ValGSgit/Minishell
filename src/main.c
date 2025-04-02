/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/02 16:18:37 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_signal_received = 0;

char *filter_evil_commands(char *input)
{
    const char *evil_commands[] = { "rm -rf /", "dd if=/dev/zero of=/dev/sdX", ":(){ :|:& };:", "mv / /dev/null", ">:|", NULL };
    int i;
    i = 0;
    while (evil_commands[i] != NULL) {
        if (ft_strcmp(input, evil_commands[i]) == 0)
	{
		printf("Why r u like that?");
            return NULL;
        }
        i++;
    }
    return input;
}
// Version with debugging functions
void	minishell_loop(t_shell *shell)
{
	char	*input;
	char	**tokens;
	char	*prompt;

	shell->cmd = NULL;
	prompt = "Minishell-> ";
	while (1)
	{
		//input = readline((const char *)update_prompt());
		// if (isatty(fileno(stdin)))
		// {
		// //prompt = update_prompt();
		// 	input = readline(prompt);
		// //free(prompt);
		// }
		// else
		// {
		// 	char *line;
		// 	line = get_next_line(fileno(stdin));
		// 	if (line)
		// 		input = ft_strtrim(line, "\n");
		// }
		setup_signals();
		input = readline(prompt);
		input = filter_evil_commands(input);
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		if (*input)
			add_history(input);
		tokens = lexer(input, shell);
		//debug_shell_state(tokens, NULL, "After Lexer");
		if (!tokens)
		{
			free(input);
			continue ;
		}
		shell->cmd = parser(tokens, shell);
		//debug_shell_state(tokens, shell->cmd, "After Parser");
		if (!shell->cmd)
		{
			free(input);
			free_tokens(tokens);
			continue ;
		}
		expand_nodes(shell->cmd, shell);
		//debug_shell_state(tokens, shell->cmd, "After Expander");
		if (shell->cmd->args[0] != NULL)
			executor(shell->cmd, shell);
		//if (shell->cmd->env)
		//	shell->env = copy_env(shell->cmd->env);
		free_cmd(shell->cmd);
		free_tokens(tokens);
		free(input);
	}
}

// Version complying with the norm (25 lines or less)
// void	minishell_loop(t_shell *shell)
// {
// 	char	*input;
// 	char	**tokens;
// 	t_cmd	*cmd;

// 	while (1)
// 	{
// 		input = readline("Minishell-> ");
// 		if (ft_strcmp(input, "exit") == 0)
// 			(free_shell(shell), exit(0));
// 		if (*input)
// 			add_history(input);
// 		tokens = lexer(input, shell);
// 		if (!tokens)
// 			free(input);
// 		cmd = parser(tokens, shell);
// 		if (!cmd)
// 		{
// 			free(input);
// 			free_tokens(tokens);
// 			continue ;
// 		}
// 		expand_nodes(cmd, shell);
// 		if (cmd->args[0] != NULL)
// 			executor(cmd, shell);
// 		free_cmd(cmd);
// 		free_tokens(tokens);
// 		free(input);
// 	}
// }

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	if (*envp)
		shell.env = copy_env(envp);
	if (!shell.env)
		return (write(2, "environment copy failed\n", 25), EXIT_FAILURE);
	shell.exit_status = 0;
	shell.is_interactive = isatty(STDIN_FILENO);
	shell.signal_status = 0;
	minishell_loop(&shell);
	rl_clear_history();
	free_env(shell.env);
	//free_cmd(shell.cmd);
	return (shell.exit_status);
}
