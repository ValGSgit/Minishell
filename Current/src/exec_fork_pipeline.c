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
		if (node->next)
		{
			if (pipe(pipe_fd) == -1)
			{
				// Close previous pipe if it's open before returning
				if (prev_pipe_in != 0)
					close(prev_pipe_in);
				return (perror("minishell: pipe"), -1);
			}
		}
		
		pid = fork_child_process(node, prev_pipe_in, pipe_fd, shell);
		if (pid == -1)
		{
			// On fork error, ensure all pipes are closed
			if (prev_pipe_in != 0)
				close(prev_pipe_in);
			if (node->next)
			{
				close(pipe_fd[READ_END]);
				close(pipe_fd[WRITE_END]);
			}
			return (handle_fork_error(pids), -1);
		}
		
		pids[i++] = pid;
		if (!node->next)
			*last_pid = pid;
			
		setup_parent_after_fork(node, &prev_pipe_in, pipe_fd);
		node = node->next;
	}
	
	// Make sure any remaining open pipe is closed
	if (prev_pipe_in != 0)
		close(prev_pipe_in);
		
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
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
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
		exit(cmd->shell->exit_status);
	}
	return (pid);
}

void	rearrange_pipes(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2])
{
	int	stderr_pipe[2];


	if (pipe(stderr_pipe) == -1)
		exit(1);
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
		{
			close(pipe_fd[WRITE_END]);
			exit(1);
		}
		close(pipe_fd[WRITE_END]);
	}
	int original_stderr = dup(STDERR_FILENO);
	if (original_stderr == -1)
		exit(1);
	if (dup2(stderr_pipe[WRITE_END], STDERR_FILENO) == -1)
		exit(1);
	close(stderr_pipe[WRITE_END]);
	
	// Fork a process to handle error output
	pid_t stderr_pid = fork();
	if (stderr_pid == -1)
		exit(1);
	
	if (stderr_pid == 0)
	{
		// Child process that reads from stderr pipe and writes to original stderr
		close(STDIN_FILENO);
		dup2(stderr_pipe[READ_END], STDIN_FILENO);
		close(stderr_pipe[READ_END]);
		
		dup2(original_stderr, STDOUT_FILENO);
		close(original_stderr);
		
		char buffer[4096];
		ssize_t bytes_read;
		
		while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			write(STDOUT_FILENO, buffer, bytes_read);
		}
		
		exit(0);
	}
	// Parent process continues with command execution
	close(stderr_pipe[READ_END]);
	close(original_stderr);
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
	int	status;

	i = 0;
	status = 0;
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
