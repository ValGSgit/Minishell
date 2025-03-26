/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:03:09 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/26 10:24:02 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

t_redir	*create_redir_node(t_redir_type type, char *file)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->type = type;
	node->file = ft_strdup(file);
	node->next = NULL;
	return (node);
}

void	add_redir_node(t_redir **head, t_redir *new_node)
{
	t_redir	*current;

	if (!*head)
	{
		*head = new_node;
	}
	else
	{
		current = *head;
		while (current->next)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

t_cmd	*create_cmd_node(void)
{
	t_cmd	*node;

	node = ft_calloc(sizeof(t_cmd), 1);
	if (!node)
		return (NULL);
	node->in_fd = STDIN_FILENO;
	node->out_fd = STDOUT_FILENO;
	node->args = NULL;
	node->redirs = NULL;
	node->next = NULL;
	return (node);
}

/* Updates the shell prompt based on the current working directory */
char	*update_prompt(void)
{
	char	cwd[1024];
	char	*colored_cwd;
	char	*prompt;

	if (!getcwd(cwd, sizeof(cwd))) // Get the current working directory
	{
		return (ft_strdup(ERROR_COLOR "Minishell-> " RESET_COLOR));
			// Error fallback
	}
	// Add color to the current working directory
	colored_cwd = ft_strjoin(PROMPT_COLOR, cwd);
	if (!colored_cwd)
		return (NULL);
	// Add arrow and reset color
	prompt = ft_strjoin(colored_cwd, " -> " RESET_COLOR);
	free(colored_cwd); // Free intermediate string
	if (!prompt)
		return (NULL);
	return (prompt);
}

void	test_file_permissions(const char *filename)
{
	if (access(filename, F_OK) != 0)
	{
		write(2, "minishell: ", 11);
		write(2, filename, ft_strlen(filename));
		write(2, ": No such file or directory\n", 28);
		return ;
	}
	if (access(filename, R_OK) != 0)
	{
		write(2, "minishell: ", 11);
		write(2, filename, ft_strlen(filename));
		write(2, ": Permission denied (read)\n", 28);
	}
	if (access(filename, W_OK) != 0)
	{
		write(2, "minishell: ", 11);
		write(2, filename, ft_strlen(filename));
		write(2, ": Permission denied (write)\n", 29);
	}
	if (access(filename, X_OK) != 0)
	{
		write(2, "minishell: ", 11);
		write(2, filename, ft_strlen(filename));
		write(2, ": Permission denied (execute)\n", 31);
	}
}