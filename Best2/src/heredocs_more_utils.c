/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_more_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:21:46 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 11:27:24 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Read heredoc input and write it to the temporary file.
 */
void	read_heredoc_input(const char *delimiter, int fd, t_cmd *cmd, bool expand_vars)
{
	char	*line;
	char	*processed_line;

	while (1)
	{
		line = readline("> ");
		if (!line) // Handle EOF (Ctrl+D) in heredocs
		{
			ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `", 2);
			ft_putstr_fd((char *)delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break;
		}
		
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		
		if (expand_vars)
		{
			processed_line = process_argument(line, cmd->shell);
			free(line); // Free original line when processed
			line = NULL; // Set to NULL to avoid potential double free
		}
		else
			processed_line = line;
			
		if (processed_line)
		{
			write(fd, processed_line, ft_strlen(processed_line));
			write(fd, "\n", 1);
			free(processed_line); // Ensure we always free the processed line
		}
	}
}

void	heredoc_child_process(t_cmd *cmd, char *clean_eof, int fd, bool expand_vars)
{
	setup_heredoc_signals();
	read_heredoc_input(clean_eof, fd, cmd, expand_vars);
	close(fd);
	free(clean_eof);
	// Clean exit from child process
	exit(0);
}

int	heredoc_parent_process(pid_t pid, t_cmd *cmd, int fd, char *clean_eof)
{
	int status;

	close(fd);
	free(clean_eof);
	waitpid(pid, &status, 0);
	
	// Handle signal termination
	if (WIFSIGNALED(status))
	{
		cmd->shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
		{
			cmd->shell->signal_status = 1;
			cmd->shell->exit_status = 130;
			return (1);
		}
	}
	// Handle normal exit
	else if (WIFEXITED(status)) 
	{
		cmd->shell->exit_status = WEXITSTATUS(status);
	}

	return (0);
}

char	*handle_expanded_line(char *arg, char *expanded_line)
{
	if (ft_strcmp(expanded_line, arg) != 0)
	{
		free(arg);
		return (expanded_line);
	}
	return (arg);
}

bool	check_delimiter_quotes(char *eof)
{
	int		i;
	bool	has_quotes;
	
	if (!eof || !*eof)
		return (true);
	
	// Check for empty quoted strings like '' or ""
	if ((eof[0] == '\'' && eof[1] == '\'' && !eof[2]) || 
		(eof[0] == '"' && eof[1] == '"' && !eof[2]))
		return (false);

	// Check for quotes anywhere in the delimiter
	i = 0;
	has_quotes = false;
	while (eof[i])
	{
		if (eof[i] == '\'' || eof[i] == '"')
			has_quotes = true;
		i++;
	}

	// If there are quotes, don't expand variables
	return (!has_quotes);
}
