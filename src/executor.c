/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:34:42 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/28 10:06:03 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

/* Restores redirections after command execution */
void	restore_redirections(t_cmd *cmd)
{
    if (cmd->in_fd != STDIN_FILENO)
        close(cmd->in_fd);
    if (cmd->out_fd != STDOUT_FILENO)
        close(cmd->out_fd);
}


static int check_file_permissions(const char *filename)
{
    if (access(filename, F_OK) != 0) // Check if the file exists
    {
        //write(2, "minishell: ", 11);
        //write(2, filename, ft_strlen(filename));
        //write(2, ": No such file or directory\n", 28);
        return (-2);
    }
    if (access(filename, R_OK) != 0) // Check read permission
    {
        write(2, "minishell: ", 11);
        write(2, filename, ft_strlen(filename));
        write(2, ": Permission denied\n", 20);
        return (-1);
    }
    return (0); // Permissions are valid
}


void apply_redirection(t_cmd *cmd) //handle dup and permissions // lacks HEREDOCS RN
{
    t_redir *redir;
    int     fd;

    redir = cmd->redirs;
    if (!redir)
        return ;
    while (redir)
    {
        if (redir->type == REDIR_IN)
        {
            fd = open(redir->file, O_RDWR, 0644);
            if (check_file_permissions(redir->file) < 0)
               exit(1); // Exit if permissions are invalid
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (redir->type == REDIR_OUT)
        {
            fd = open(redir->file, O_RDWR | O_CREAT, 0644);
            if (access(redir->file, W_OK) != 0)
             {
                write(2, "minishell: ", 11);
                write(2, redir->file, ft_strlen(redir->file));
                write(2, ": Permission denied (write)\n", 29);
                exit(1); // Exit if permissions are invalid
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if (redir->type == REDIR_APPEND)
        {
            fd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT , 0644);
            if (access(redir->file, W_OK) != 0)
            {
                write(2, "minishell: ", 11);
                write(2, redir->file, ft_strlen(redir->file));
                write(2, ": Permission denied (write)\n", 29);
                exit(1); // Exit if permissions are invalid
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        redir = redir->next;
    }    
}

/* Executes a single command (builtin or external) */
static void execute_single_command(t_cmd *cmd, t_shell *shell)
{
    pid_t pid;
    
    if (is_builtin(cmd->args[0]))
    {
        execute_builtin(cmd);
        return;
    }
    // Check execute permissions for the command
    if (access(cmd->args[0], X_OK) != 0)
    {
        if (check_file_permissions(cmd->args[0]) == -1)
        {
            // write(2, "minishell: ", 11);
            // write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            // write(2, ": Permission denied (execute)\n", 30);
            shell->exit_status = 126; // Set exit status for permission error
            return ;
        }
        else if (check_file_permissions(cmd->args[0]) == -2)
        {
            shell->exit_status = 127; // Set exit status for file not found
            return ;
        }
        write(2, "minishell: ", 11);
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": command not found\n", 20);
        shell->exit_status = 127; // Set exit status for permission error
        return;
    }
    pid = fork();
    if (pid == 0)
    {
        apply_redirection(cmd); // Set up redirections with permission checks
        execve(cmd->args[0], cmd->args, shell->env);
        perror(cmd->args[0]); // Print error if execve fails
        exit(127); // Exit with 127 if command not found
    }
    else
    {
        waitpid(pid, &shell->exit_status, 0);
        if (WIFEXITED(shell->exit_status))
        shell->exit_status = WEXITSTATUS(shell->exit_status);
        else
        shell->exit_status = 1; // Handle signals
        restore_redirections(cmd); // Restore redirections
    }
}


static void execute_pipeline(t_cmd *cmd, t_shell *shell)
{
    int pipe_fd[2];
    int prev_pipe_in = 0;
    pid_t pid;
    
    while (cmd)
    {
        if (cmd->next)
        {
            if (pipe(pipe_fd) == -1) // Create a pipe for the next command
            {
                perror("pipe");
                return;
            }
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return;
        }
        if (pid == 0) // Child process
        {
            if (prev_pipe_in != 0) // Redirect input from the previous pipe
            {
                dup2(prev_pipe_in, STDIN_FILENO);
                close(prev_pipe_in);
            }
            if (cmd->next) // Redirect output to the current pipe
            {
                dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
                close(pipe_fd[WRITE_END]);
                close(pipe_fd[READ_END]);
            }
            
            // Check execute permissions for the command
            if (access(cmd->args[0], X_OK) != 0)
            {
                // write(2, "minishell: ", 11);
                // write(2, cmd->args[0], ft_strlen(cmd->args[0]));
                // write(2, ": Permission denied (execute)\n", 30);
                exit(126); // Exit with 126 for permission error
            }
            
            apply_redirection(cmd); // Set up redirections with permission checks
            execve(cmd->args[0], cmd->args, shell->env);
            perror(cmd->args[0]); // Print error if execve fails
            exit(127);
        }
        else // Parent process
        {
            if (prev_pipe_in != 0) // Close the previous pipe's read end
            close(prev_pipe_in);
            if (cmd->next) // Close the current pipe's write end
            close(pipe_fd[WRITE_END]);
            prev_pipe_in = cmd->next ? pipe_fd[READ_END] : 0; // Update for the next iteration
            cmd = cmd->next;
        }
    }
    while (wait(NULL) > 0);
}

/* Main executor function */
void	executor(t_cmd *cmd, t_shell *shell)
{
    if (!cmd)
        return;
    if (!cmd->next) // Single command
        execute_single_command(cmd, shell);
    else // Pipeline
        execute_pipeline(cmd, shell);
}
