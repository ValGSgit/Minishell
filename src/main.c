/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/07 16:20:35 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_signal_received = 0;


// Version with debugging functions
int		handle_input(t_shell *shell, char *input)
{
	char	**tokens;
	
	if (ft_strncmp(input, "exit", ft_strlen(input) + 5) == 0)
		return (1);
	if (*input)
		add_history(input);
	tokens = lexer(input);
	if (!tokens)
	{
		free(input);
		return (0);
	}
	shell->cmd = parser(tokens, shell);
	if (!shell->cmd)
		return(free(input),free_tokens(tokens), 0);
	expand_nodes(shell->cmd, shell);
	if (shell->cmd->args[0] != NULL)
		executor(shell->cmd, shell);
	free_cmd(shell->cmd);
	free_tokens(tokens);
	free(input);
	return (0);
}

void	minishell_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;
	
	shell->cmd = NULL;
	prompt = "Minishell-> ";
	while (1)
	{
		setup_signals();
		// //input = readline((const char *)update_prompt());
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
		input = readline(prompt);
		if (handle_input(shell, input) == 1)
		{
			free(input);
			break ;
		}
	}
}


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
