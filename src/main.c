/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/26 14:59:14 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

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
            printf("Redirection file: %s\n", current->redirs->file);
			if (current->redirs->type == REDIR_IN)
				printf("Type: REDIR_IN\n");
			else if (current->redirs->type == REDIR_OUT)
				printf("Type: REDIR_OUT\n");
			else if (current->redirs->type == REDIR_APPEND)
				printf("Type: REDIR_APPEND\n");
			else if (current->redirs->type == REDIR_HEREDOC)
				printf("Type: REDIR_HEREDOC\n");
			if (current->redirs->next)
			{
				printf("Next redirection file: %s\n", current->redirs->next->file);
				if (current->redirs->next->type == REDIR_IN)
					printf("Type: REDIR_IN\n");
				else if (current->redirs->next->type == REDIR_OUT)
					printf("Type: REDIR_OUT\n");
				else if (current->redirs->next->type == REDIR_APPEND)
					printf("Type: REDIR_APPEND\n");
				else if (current->redirs->next->type == REDIR_HEREDOC)
					printf("Type: REDIR_HEREDOC\n");
			}
		}
        current = current->next;
    }
}

void	minishell_loop(t_shell *shell)
{
	char	*input;
	char	**tokens;
	t_cmd	*cmd;
	char	*prompt;

	cmd = NULL;
	prompt = "Minishell-> ";
	while (1)
	{
		//prompt = update_prompt();
		input = readline(prompt);
		//free(prompt);
		if (!input)
			break ;
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		if (*input)
			add_history(input);
		tokens = lexer(input, shell); //Handle quotes and metacharacters
		if (!tokens)
		{
			free(input);
			continue ;
		}
		cmd = parser(tokens); // Parse tokens into commands and redirs into nodes
		if (!cmd)
		{
			free(input);
			free_tokens(tokens);
			continue ;
		}
		print_parsed_command(cmd);
		expander(cmd, shell); // Quote removal and var expansion
		print_parsed_command(cmd);
		executor(cmd, shell);
		shell->exit_status = cmd->exit_status;
		free_cmd(cmd);
		free_tokens(tokens);
		free(input);
	}
	rl_clear_history();
}

char	**copy_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			free_env(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
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
	shell.is_interactive = isatty(STDIN_FILENO);
	shell.signal_status = 0;
	setup_signals();
	shell.exit_status = 0;
	minishell_loop(&shell);
	rl_clear_history();
	free_env(shell.env);
	return (shell.exit_status);
}
