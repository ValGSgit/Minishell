/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/02 13:19:26 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_signal_received = 0;

void	print_parsed_command(t_cmd *cmd)
{
    t_cmd	*current;
    int		i;

    current = cmd;
    while (current)
    {
        printf("Command: %s\n", current->args[0]);
        if (current->args)
        {
            printf("Arguments:\n");
            i = 1;
            while (current->args[i])
                printf("  - %s\n", current->args[i++]);
        }
        if (current->redirs)
		{
            printf("Input redirection: %s\n", current->redirs->file);
            printf("Output redirection: %s\n", current->redirs->file);
		}
        current = current->next;
    }
}

// Version with debugging functions
void	minishell_loop(t_shell *shell)
{
	char	*input;
	char	**tokens;
	//t_cmd	*cmd;
	char	*prompt;

	shell->cmd = NULL;
	prompt = "Minishell-> ";
	while (1)
	{
		input = readline(prompt);
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
		if (!input)
			break ;
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
		if (shell->cmd->env)
			shell->env = copy_env(shell->cmd->env);
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
	setup_signals();
	minishell_loop(&shell);
	rl_clear_history();
	free_env(shell.env);
	//free_cmd(shell.cmd);
	return (shell.exit_status);
}
