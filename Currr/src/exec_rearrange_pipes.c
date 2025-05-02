/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_rearrange_pipes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iionescu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 08:16:41 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/02 08:16:43 by iionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	setup_std_pipes(t_cmd *cmd, int prev_pipe_in,
	int pipe_fd[2], int stderr_pipe[2])
{
	if (pipe(stderr_pipe) == -1)
		exit(1);
	if (prev_pipe_in != 0)
	{
		if (dup2(prev_pipe_in, STDIN_FILENO) == -1)
			exit(1);
		close(prev_pipe_in);
	}
	if (cmd->next)
	{
		close(pipe_fd[READ_END]);
		if (dup2(pipe_fd[WRITE_END], STDOUT_FILENO) == -1)
		{
			close(pipe_fd[WRITE_END]);
			exit(1);
		}
		close(pipe_fd[WRITE_END]);
	}
}

static void	fork_stderr(int stderr_pipe[2],
	int *original_stderr, pid_t *stderr_pid)
{
	*original_stderr = dup(STDERR_FILENO);
	if (*original_stderr == -1
		|| dup2(stderr_pipe[WRITE_END], STDERR_FILENO) == -1)
		exit(1);
	close(stderr_pipe[WRITE_END]);
	*stderr_pid = fork();
	if (*stderr_pid == -1)
		exit(1);
}

static void	handle_stderr_printing(int stderr_pipe[2], int original_stderr)
{
	char	buffer[4096];
	ssize_t	bytes_read;

	close(STDIN_FILENO);
	dup2(stderr_pipe[READ_END], STDIN_FILENO);
	close(stderr_pipe[READ_END]);
	dup2(original_stderr, STDOUT_FILENO);
	close(original_stderr);
	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		write(STDOUT_FILENO, buffer, bytes_read);
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	}
	exit(0);
}

void	rearrange_pipes(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2])
{
	int		stderr_pipe[2];
	int		original_stderr;
	pid_t	stderr_pid;

	setup_std_pipes(cmd, prev_pipe_in, pipe_fd, stderr_pipe);
	fork_stderr(stderr_pipe, &original_stderr, &stderr_pid);
	if (stderr_pid == 0)
	{
		handle_stderr_printing(stderr_pipe, original_stderr);
	}
	close(stderr_pipe[READ_END]);
	close(original_stderr);
}
