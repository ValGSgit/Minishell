#include "../includes/minishell.h"

void	waitpid_for_single_command(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
}

void	execute_builtin_or_exit(t_cmd *cmd)
{
	// Execute builtin and exit with its exit status
	execute_builtin(cmd);
	//exit(cmd->shell->exit_status);
}

void	rearrange_pipes(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2])
{
	if (prev_pipe_in != 0)
	{
		dup2(prev_pipe_in, STDIN_FILENO);
		close(prev_pipe_in);
	}
	if (cmd->next)
	{
		dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
		close(pipe_fd[WRITE_END]);
		close(pipe_fd[READ_END]);
	}
}
void	wait_for_pipeline(pid_t *pids, int count, pid_t last_pid,
	t_shell *shell)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (pids[i] == last_pid)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				shell->exit_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					write(STDERR_FILENO, "\n", 1);
			}
		}
		i++;
	}
	free(pids);
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
	else
		*prev_pipe_in = 0;
}
