#include "../includes/minishell.h"

void	executor(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
	{
		shell->exit_status = 0;
		return;
	}
	if (!cmd->next)
		execute_single_command(cmd, shell);
	else
		execute_pipeline(cmd, shell);
}
void execute_single_command(t_cmd *cmd, t_shell *shell)
{
	pid_t pid;

	if ((!cmd->args && cmd->redirs) || (cmd->args && cmd->args[0] && is_builtin(cmd->args[0])))
	{
		if (cmd->redirs)
			apply_redirection(cmd);
		if (cmd->args && is_builtin(cmd->args[0]))
			execute_builtin(cmd);
		dup2(cmd->out_fd, STDOUT_FILENO);
		dup2(cmd->in_fd, STDIN_FILENO);
		return;
	}
	pid = fork();
	if (pid == 0)
	{
		if (cmd->redirs)
			apply_redirection(cmd);
		execute_external_command(cmd, shell);
	}
	waitpid_for_single_command(pid, shell);
}

void execute_external_command(t_cmd *cmd, t_shell *shell)
{
	struct stat sb;
	char *exec_path;
	char *cmd_name;
	
	exec_path = NULL;
	if (cmd->args && cmd->args[0])
	{
		if (cmd->args[0] && shell->path_unset && ft_strchr(cmd->args[0], '/') == NULL)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(127);
		}
		external_cmd_checks(cmd);
		if (stat(cmd->args[0], &sb) == 0 && S_ISDIR(sb.st_mode)) //dir check
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
		if (access(cmd->args[0], X_OK) != 0)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}		
		exec_path = ft_strdup(cmd->args[0]);
		if (!exec_path)
			exit(1);
		cmd_name = get_clean_cmd_name(cmd->args[0]);
		cmd->args[0] = cmd_name;
		execve(exec_path, cmd->args, shell->env);
		safe_free(exec_path);
		execve_error(cmd);
	}
	exit(0); /* Ensure we exit if there are no args */
}

char	*get_clean_cmd_name(const char *path)
{
	const char	*last_slash;

	if (!path)
		return (NULL);
	last_slash = ft_strrchr(path, '/');
	if (last_slash)
		return ((char *)(last_slash + 1));
	return ((char *)path);
}


void	external_cmd_checks(t_cmd *cmd)
{
	if (access(cmd->args[0], F_OK) != 0)
	{
		if (ft_strchr(cmd->args[0], '/') != NULL) // Path was specified but file doesn't exist
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(127);
		}
		else //Simple command that wasn't found in PATH
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
	}
}
void	execve_error(t_cmd *cmd)
{

	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	exit(126);
}

