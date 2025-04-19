#include "../includes/minishell.h"


void	execute_pipeline(t_cmd *cmd, t_shell *shell)
{
	int		count;
	pid_t	last_pid;
	pid_t	*pids;
	int		fork_result;

	count = count_pipeline_cmds(cmd);
	pids = allocate_pid_array(count);
	if (!pids)
		return ;
	fork_result = fork_pipeline_commands(cmd, shell, pids, &last_pid);
	if (fork_result == -1 || fork_result == 1)
		return ;
	wait_for_pipeline(pids, fork_result, last_pid, shell);
}

void	only_redir_fork(t_cmd *cmd)
{
	int		status;
	pid_t	pid;
	if (!cmd->args && cmd->redirs)
	{
		pid = fork();
		if (pid == 0)
		{
			apply_redirection(cmd);
			exit(0);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			cmd->shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			cmd->shell->exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				write(STDERR_FILENO, "\n", 1);
		}
		return;
	}
}

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
		if (handle_only_redirection(cmd))
			return (1);
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
			apply_redirection(cmd);
		if (is_builtin(cmd->args[0]))
			execute_builtin_or_exit(cmd);
		execute_external_command(cmd, shell);
	}
	return (pid);
}

pid_t	*allocate_pid_array(int count)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
		perror("minishell: malloc");
	return (pids);
}

int	count_pipeline_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}