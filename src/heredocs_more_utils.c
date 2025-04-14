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
		if (ft_strlen(line) == ft_strlen(delimiter) && ft_strcmp(line,
				delimiter) == 0)
		{
			free(line);
			break ;
		}
		processed_line = process_line(line, cmd, expand_vars);
		if (processed_line)
		{
			write(fd, processed_line, ft_strlen(processed_line));
			write(fd, "\n", 1);
			if (processed_line != line)
				free(processed_line);
		}
	}
}

void	heredoc_child_process(t_cmd *cmd, char *clean_eof, int fd, bool expand_vars)
{
	setup_heredoc_signals();
	read_heredoc_input(clean_eof, fd, cmd, expand_vars);
	close(fd);
	free(clean_eof);
	exit(0);
}

int	heredoc_parent_process(t_cmd *cmd, int fd, char *clean_eof)
{
	int status;

	close(fd);
	free(clean_eof);
	waitpid(-1, &status, 0);
	if (WIFSIGNALED(status))
	{
		cmd->shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
		{
			cmd->shell->signal_status = 1;
			return (1);
		}
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
	return (!((eof[0] == '\'' && eof[ft_strlen(eof) - 1] == '\'')
			|| (eof[0] == '\"' && eof[ft_strlen(eof) - 1] == '\"')));
}
