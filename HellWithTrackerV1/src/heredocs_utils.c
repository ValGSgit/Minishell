/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:38:19 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/23 13:55:12 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	heredoc_child_process(t_cmd *cmd, char *clean_eof, int fd,
			bool expand_vars)
{
	setup_heredoc_signals();
	read_heredoc_input(clean_eof, fd, cmd, expand_vars);
	free(clean_eof);
	close(fd);
	exit(0);
}

int	heredoc_parent_process(pid_t pid, t_cmd *cmd, int fd, char *clean_eof)
{
	int	status;

	close(fd);
	waitpid(pid, &status, 0);
	free(clean_eof);
	
	if (WIFEXITED(status))
	{
		cmd->shell->exit_status = WEXITSTATUS(status);
		if (WEXITSTATUS(status) == 130)
		{
			cmd->shell->signal_status = 1;
			return (1);
		}
	}
	else if (WIFSIGNALED(status))
	{
		cmd->shell->signal_status = 1;
		cmd->shell->exit_status = 128 + WTERMSIG(status);
		return (1);
	}
	return (0);
}
