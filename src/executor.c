/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:34:42 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/02 16:23:20 by vagarcia         ###   ########.fr       */
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

int	check_file_permissions(const char *filename)
{
    struct stat sb;

    if (access(filename, F_OK) != 0)
    {
        perror(filename);
        return (127); // File not found
    }
    if (stat(filename, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        fprintf(stderr, "%s: Is a directory\n", filename);
        return (126); // Is a directory
    }
    if (access(filename, R_OK) != 0)
    {
        fprintf(stderr, "%s: Permission denied\n", filename);
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
        perror(redir->file);
        exit(1); // Exit with error code 1 for missing files
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void	handle_redirection_out(t_redir *redir, int append)
{
    int fd;
    int writable;

    if (append)
        fd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
    else
        fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    writable = access(redir->file, W_OK);
    if (writable != 0)
    {
        write(2, " Permission denied\n", 20);
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
    execute_builtin(cmd);
    restore_redirections(cmd);
    exit(0);
}

void	execute_external_command(t_cmd *cmd, t_shell *shell)
{
    struct stat sb;

    if (stat(cmd->args[0], &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        fprintf(stderr, "%s: Is a directory\n", cmd->args[0]);
        exit(126); // Is a directory
    }
    if (access(cmd->args[0], X_OK) != 0)
    {
        perror(cmd->args[0]);
        exit(127); // Command not found or permission denied
    }
    execve(cmd->args[0], cmd->args, shell->env);
    perror(cmd->args[0]);
    exit(127);
}

void	execute_single_command(t_cmd *cmd, t_shell *shell)
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
    if (WIFEXITED(status)) // Check if the child exited normally
        shell->exit_status = WEXITSTATUS(status); // Extract the exit code
    else if (WIFSIGNALED(status)) // Check if the child was terminated by a signal
        shell->exit_status = 128 + WTERMSIG(status); // Set exit code to 128 + signal number
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

void	execute_pipeline(t_cmd *cmd, t_shell *shell)
{
    int pipe_fd[2];
    int prev_pipe_in = 0;
    pid_t pid;
    int status;

    while (cmd)
    {
        if (cmd->next && pipe(pipe_fd) == -1)
        {
            perror("pipe");
            return;
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return;
        }
        if (pid == 0) // Child process
        {
            if (prev_pipe_in != 0) // If there's input from a previous pipe
            {
                dup2(prev_pipe_in, STDIN_FILENO);
                close(prev_pipe_in);
            }
            if (cmd->next) // If there's a next command, set up output to pipe
            {
                dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
                close(pipe_fd[WRITE_END]);
                close(pipe_fd[READ_END]);
            }
            apply_redirection(cmd); // Apply redirections
            if (is_builtin(cmd->args[0]))
                execute_builtin_or_exit(cmd);
            execute_external_command(cmd, shell);
        }
        // Parent process
        if (prev_pipe_in != 0)
            close(prev_pipe_in); // Close the previous pipe's read end
        if (cmd->next)
        {
            close(pipe_fd[WRITE_END]); // Close the write end of the current pipe
            prev_pipe_in = pipe_fd[READ_END]; // Save the read end for the next command
        }
        else
            prev_pipe_in = 0; // Reset for the last command
        cmd = cmd->next;
    }
    while (wait(&status) > 0) // Wait for all child processes
    {
        if (WIFEXITED(status)) // Check if the child exited normally
            shell->exit_status = WEXITSTATUS(status); // Extract the exit code
        else if (WIFSIGNALED(status)) // Check if the child was terminated by a signal
            shell->exit_status = 128 + WTERMSIG(status); // Set exit code to 128 + signal number
    }
}

void	executor(t_cmd *cmd, t_shell *shell)
{
    if (!cmd)
        return;
    if (!cmd->next)
        execute_single_command(cmd, shell);
    else
        execute_pipeline(cmd, shell);
    shell->env = copy_env(cmd->env);
}
