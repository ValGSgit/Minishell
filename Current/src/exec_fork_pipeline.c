/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork_pipeline.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:50:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 18:17:30 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	create_pipe_if_needed(int *pipe_fd, int *prev_pipe_in)
{
	if (pipe(pipe_fd) == -1)
	{
		if (*prev_pipe_in != 0)
			close(*prev_pipe_in);
		return (perror("minishell: pipe"), -1);
	}
	return (0);
}

static int	handle_fork_failure(int prev_pipe_in,
	t_cmd *node, pid_t *pids, int *pipe_fd)
{
	if (prev_pipe_in != 0)
		close(prev_pipe_in);
	if (node->next)
	{
		close(pipe_fd[READ_END]);
		close(pipe_fd[WRITE_END]);
	}
	handle_fork_error(pids);
	return (-1);
}

int	fork_pipeline_commands(t_cmd *cmd, t_shell *shell, pid_t *pids,
	pid_t *last_pid)
{
	int		pipe_fd[2];
	int		prev_pipe_in;
	int		i;
	pid_t	pid;
	t_cmd	*node;

	node = cmd;
	prev_pipe_in = 0;
	i = 0;
	while (node)
	{
		if (node->next && create_pipe_if_needed(pipe_fd, &prev_pipe_in) == -1)
			return (-1);
		pid = fork_child_process(node, prev_pipe_in, pipe_fd, shell);
		if (pid == -1)
			return (handle_fork_failure(prev_pipe_in, node, pids, pipe_fd));
		pids[i++] = pid;
		if (!node->next)
			*last_pid = pid;
		setup_parent_after_fork(node, &prev_pipe_in, pipe_fd);
		node = node->next;
	}
	if (prev_pipe_in != 0)
		close(prev_pipe_in);
	return (i);
}

pid_t	fork_child_process(t_cmd *cmd, int ppi, int pipe_fd[2], t_shell *shell)
{
	pid_t	pid;

	ignore_signals();
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		rearrange_pipes(cmd, ppi, pipe_fd);
		if (cmd->redirs)
			apply_redirection(cmd, true);
		if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd);
			close(0);
			close(1);
			exit(cmd->shell->exit_status);
		}
		if (cmd->args && cmd->args[0] && cmd->args[0][0]
			&& cmd->args[0][0] != '\0')
		{
			execute_external_command(cmd, shell);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			if (ppi != -1) 
                close(ppi);
			close(0);
			close(1);
		}
		forked_exit(cmd->shell->exit_status);
	}
	return (pid);
}

void	setup_parent_after_fork(t_cmd *cmd, int *prev_pipe_in, int pipe_fd[2])
{
	if (*prev_pipe_in != 0)
		close(*prev_pipe_in);
	if (cmd->next)
	{
		close(pipe_fd[WRITE_END]);
		*prev_pipe_in = pipe_fd[READ_END];
	}
}
