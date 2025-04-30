/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/01/31 15:03:09 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/27 11:30:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	create_redir_node(t_cmd *cmd, int type, char *file)
{
	t_redir *node;
	t_redir *tmp;

	node = xmalloc(sizeof(t_redir));
	if (!node)
		return ;
	node->type = type;
	node->prefile = ft_strdup(file);
	if (!node->prefile)
	{
		safe_free(node);
		return ;
	}
	node->file = process_argument(file, cmd->shell);
	node->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = node;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

t_cmd	*create_cmd_node(void)
{
	t_cmd *node;

	node = xmalloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->in_fd = -1;
	node->out_fd = -1;
	node->args = NULL;
	node->redirs = NULL;
	node->next = NULL;
	node->in_file = NULL;
	node->out_file = NULL;
	node->syntax_error = false;
	return (node);
}

/* Updates the shell prompt based on the current working directory */
char	*update_prompt(void)
{
	static char cwd[1024];
	char *prompt;
	char *result;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		return (ft_strdup("Minishell-> "));
	}
	prompt = ft_strjoin(cwd, " -> ");
	if (!prompt)
		return (NULL);
	result = ft_strdup(prompt);
	if (!result)
	{
		free(prompt);
		return (NULL);
	}
	free(prompt);
	return (result);
}

/**
 * Report a syntax error and set the shell exit status to 2
 */
void	handle_syntax_error(char *token, t_shell *shell)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	shell->exit_status = 2;
}

void cleanup_shell(t_shell *shell)
{
    // Free any command structures
    if (shell->cmd)
    {
        free_cmd(shell->cmd);
        shell->cmd = NULL;
    }
    
    // Free environment variables
    if (shell->env)
    {
        free_env(shell->env);
        shell->env = NULL;
    }
    
    // Clear readline history to prevent memory leaks
    rl_clear_history();
    
    // This additional function call helps address readline internal memory leaks
    rl_free_line_state();
    
    // Reset readline internal completion state to prevent memory leaks
    rl_attempted_completion_over = 0;
    rl_completion_type = 0;
    
    // Ensure all file descriptors are closed
    // Command file descriptors are already handled by free_cmd
}