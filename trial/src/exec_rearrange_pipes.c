/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_rearrange_pipes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 08:16:41 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/03 17:25:51 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	rearrange_pipes(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2])
{
	if (prev_pipe_in != 0)
	{
		if (dup2(prev_pipe_in, STDIN_FILENO) == -1)
		{
			forked_exit(1, cmd);
		}
		close(prev_pipe_in);
	}
	if (cmd->next)
	{
		close(pipe_fd[READ_END]);
		if (dup2(pipe_fd[WRITE_END], STDOUT_FILENO) == -1)
		{
			close(pipe_fd[WRITE_END]);
			forked_exit(1, cmd);
		}
		close(pipe_fd[WRITE_END]);
	}
}
