/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:34:42 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/13 12:47:23 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	restore_redirections(t_cmd *cmd)
{
    if (cmd->in_fd != STDIN_FILENO)
    {
        close(cmd->in_fd);
        dup2(cmd->in_fd, STDIN_FILENO);
    }
    if (cmd->out_fd != STDOUT_FILENO)
    {
        close(cmd->out_fd);
        dup2(cmd->out_fd, STDOUT_FILENO);
    }
}

// Helper function to get the base command name (for error messages)
char	*get_cmd_basename(char *path)
{
    char *basename;
    
    if (!path)
        return (NULL);
    basename = ft_strrchr(path, '/');
    if (basename)
        return (basename + 1);
    return (path);
}

int	check_file_permissions(const char *filename)
{
    struct stat sb;

    if (access(filename, F_OK) != 0)
    {
        ft_putstr_fd((char *)filename, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        return (127); // File not found
    }
    if (stat(filename, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        ft_putstr_fd((char *)filename, 2);
        ft_putstr_fd(": Is a directory\n", 2);
        return (126); // Is a directory
    }
    if (access(filename, R_OK) != 0)
    {
        ft_putstr_fd((char *)filename, 2);
        ft_putstr_fd(": Permission denied\n", 2);
        return (126); // Permission denied
    }
    return (0); // Permissions are valid
}

void	handle_redirection_in(t_redir *redir)
{
    int fd;

    fd = open(redir->file, O_RDONLY);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(redir->file, 2);
        ft_putstr_fd(": ", 2);
        ft_putstr_fd(strerror(errno), 2);
        ft_putstr_fd("\n", 2);
        exit(1); // Exit with error code 1 for file errors
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void	handle_redirection_out(t_redir *redir, int append)
{
    int fd;

    if (append)
        fd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
    else
        fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
   
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(redir->file, 2);
        ft_putstr_fd(": ", 2);
        ft_putstr_fd(strerror(errno), 2);
        ft_putstr_fd("\n", 2);
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void	apply_redirection(t_cmd *cmd)
{
    t_redir *redir;

    redir = cmd->redirs;
    while (redir)
    {
        if (redir->type == REDIR_IN)
            handle_redirection_in(redir);
        else if (redir->type == REDIR_OUT)
            handle_redirection_out(redir, 0);
        else if (redir->type == REDIR_APPEND)
            handle_redirection_out(redir, 1);
        else if (redir->type == REDIR_HEREDOC)
            handle_redirection_in(redir);
        redir = redir->next;
    }
}

void	execute_builtin_or_exit(t_cmd *cmd)
{
    int prev_exit_status;
    
    // Save exit status in case the builtin changes it
    prev_exit_status = cmd->shell->exit_status;
    
    // Execute the builtin
    execute_builtin(cmd);
    
    // Exit with the builtin's exit status
    (void)prev_exit_status;
    
    exit(cmd->shell->exit_status);
}

void execute_external_command(t_cmd *cmd, t_shell *shell)
{
    struct stat sb;
    
    // Handle empty command case
    if (!cmd->args[0] || !*cmd->args[0])
        exit(0);
    
    // Check if PATH has been unset
    if (shell->path_unset && ft_strchr(cmd->args[0], '/') == NULL)
    {
        ft_putstr_fd(get_cmd_basename(cmd->args[0]), 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        exit(127);
    }

    // Check file existence
    if (access(cmd->args[0], F_OK) != 0)
    {
        if (ft_strchr(cmd->args[0], '/') != NULL)
        {
            // Path was specified but file doesn't exist

            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            exit(127);
        }
        else
        {
            // Simple command that wasn't found in PATH

            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            exit(127);
        }
    }

    // Check if it's a directory
    if (stat(cmd->args[0], &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd(": Is a directory\n", 2);
        exit(126);
    }
    
    // Check execute permission
    if (access(cmd->args[0], X_OK) != 0)
    {
        ft_putstr_fd(get_cmd_basename(cmd->args[0]), 2);
        ft_putstr_fd(": Permission denied\n", 2);
        exit(126);
    }
    
    // Execute command
    execve(cmd->args[0], cmd->args, shell->env);
    
    // Should not reach here unless execve fail
    ft_putstr_fd(get_cmd_basename(cmd->args[0]), 2);
    ft_putstr_fd(": ", 2);
    ft_putstr_fd(strerror(errno), 2);
    ft_putstr_fd("\n", 2);
    exit(126);
}

void execute_single_command(t_cmd *cmd, t_shell *shell)
{
    pid_t pid;
    int status;
    int is_built;

    is_built = is_builtin(cmd->args[0]);
    if (is_built && !cmd->next && !cmd->redirs)
    {
        execute_builtin(cmd);
        restore_redirections(cmd);
        return;
    }
    pid = fork();
    if (pid == 0)
    {
        apply_redirection(cmd);
        if (is_built)
            execute_builtin_or_exit(cmd);
        execute_external_command(cmd, shell);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        shell->exit_status = 128 + WTERMSIG(status);
        if (WTERMSIG(status) == SIGINT)
            write(STDERR_FILENO, "\n", 1);
    }
    restore_redirections(cmd);
}

void	setup_pipeline(t_cmd *cmd, int *pipe_fd, int *prev_pipe_in)
{
    if (cmd->next)
    {
        pipe(pipe_fd);
        dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
        close(pipe_fd[WRITE_END]);
    }
    if (*prev_pipe_in != 0)
    {
        dup2(*prev_pipe_in, STDIN_FILENO);
        close(*prev_pipe_in);
    }
}

void pipe_exit_status(pid_t last_pid, t_shell *shell)
{
    pid_t   wpid;
    int     status;

    while ((wpid = wait(&status)) > 0)
    {
        if (wpid == last_pid)
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
    }
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
        // Child process
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
        apply_redirection(cmd);
        if (is_builtin(cmd->args[0]))
            execute_builtin_or_exit(cmd);
        execute_external_command(cmd, shell);
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
	else
		*prev_pipe_in = 0;
}

void	execute_pipeline(t_cmd *cmd, t_shell *shell)
{
    int pipe_fd[2];
    int prev_pipe_in = 0;
    pid_t   pid;
    pid_t   last_pid = 0;
    t_cmd   *cmd_head = cmd;
    int     cmd_count = 0;
    pid_t   *pids;
    int     status;
    
    // Count commands in pipeline
    while (cmd)
    {
        cmd_count++;
        cmd = cmd->next;
    }
    
    // Allocate memory for pid array
    pids = malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
        return;
    
    // Reset cmd and create all processes first
    cmd = cmd_head;
    cmd_count = 0;
    
    while (cmd)
    {
        if (cmd->next && pipe(pipe_fd) == -1)
        {
            perror("minishell: pipe");
            free(pids);
            return;
        }
        
        pid = fork_child_process(cmd, prev_pipe_in, pipe_fd, shell);
        if (pid == -1)
        {
            perror("minishell: fork");
            free(pids);
            return;
        }
        
        pids[cmd_count++] = pid;
        if (!cmd->next)
            last_pid = pid;
            
        setup_parent_after_fork(cmd, &prev_pipe_in, pipe_fd);
        cmd = cmd->next;
    }
    
    // Wait for all processes and properly track exit status
    for (int i = 0; i < cmd_count; i++)
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
    }
    
    free(pids);
}
   
void	executor(t_cmd *cmd, t_shell *shell)
{
    if (!cmd || !cmd->args || !cmd->args[0])
    {
        // Handle empty command case
        shell->exit_status = 0;
        return;
    }
    
    if (!cmd->next)
        execute_single_command(cmd, shell);
    else
        execute_pipeline(cmd, shell);
}
