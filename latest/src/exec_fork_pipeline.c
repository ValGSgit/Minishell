/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork_pipeline.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:50:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/29 15:47:40 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	fork_pipeline_commands(t_cmd *cmd, t_shell *shell, pid_t *pids,
	pid_t *last_pid)
{
	int		pipe_fd[2];
	int		prev_pipe_in;
	int		i;
	pid_t	pid;

	prev_pipe_in = 0;
	i = 0;
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
			return (perror("minishell: pipe"), -1);
		pid = fork_child_process(cmd, prev_pipe_in, pipe_fd, shell);
		if (pid == -1)
			return (handle_fork_error(pids), -1);
		pids[i++] = pid;
		if (!cmd->next)
			*last_pid = pid;
		setup_parent_after_fork(cmd, &prev_pipe_in, pipe_fd);
		cmd = cmd->next;
	}
	return (i);
}

pid_t fork_child_process(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2], t_shell *shell)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		rearrange_pipes(cmd, prev_pipe_in, pipe_fd);
		if (cmd->redirs)
			apply_redirection(cmd, true);
		if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd);
			exit(cmd->shell->exit_status);
		}
		if (cmd->args && cmd->args[0] && cmd->args[0][0] && cmd->args[0][0] != '\0')
			execute_external_command(cmd, shell);
		exit(0);
	}
	return (pid);
}

void	rearrange_pipes(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2])
{
	if (prev_pipe_in != 0)
	{
		if (dup2(prev_pipe_in, STDIN_FILENO) == -1)
		{
			exit(1);
		}
		close(prev_pipe_in);
	}
	if (cmd->next)
	{
		close(pipe_fd[READ_END]);
		if (dup2(pipe_fd[WRITE_END], STDOUT_FILENO) == -1)
			exit(1);
		close(pipe_fd[WRITE_END]);
	}
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

void	wait_for_pipeline(pid_t *pids, int count, pid_t last_pid,
	t_shell *shell)
{
	int	i;
	int	status = 

	i = 0;
	while (i < count)
	{
		if (pids[i] == last_pid)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				shell->exit_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					write(STDERR_FILENO, "\n", 1);
			}
		}
		else
			waitpid(pids[i], NULL, 0);
		i++;
	}
	free(pids);
}
