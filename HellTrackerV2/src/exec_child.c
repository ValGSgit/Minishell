/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:12:45 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/23 13:15:31 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_child(t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (cmd->in_fd != STDIN_FILENO)
		{
			dup2(cmd->in_fd, STDIN_FILENO);
			close(cmd->in_fd);
		}
		if (cmd->out_fd != STDOUT_FILENO)
		{
			dup2(cmd->out_fd, STDOUT_FILENO);
			close(cmd->out_fd);
		}
		execute_builtin_or_exit(cmd);
		execute_external_command(cmd, cmd->shell);
	}
	return (pid);
}

void	execute_builtin_or_exit(t_cmd *cmd)
{
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
		exit(cmd->shell->exit_status);
	}
}

// get_cmd_path function removed - using the one from cmd_path.c instead

void	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	struct stat	sb;
	char		*cmd_path;

	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (stat(cmd->args[0], &sb) == 0 && S_ISDIR(sb.st_mode))
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
		if (access(cmd->args[0], F_OK) == -1)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(127);
		}
		if (access(cmd->args[0], X_OK) == -1)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
		execve(cmd->args[0], cmd->args, shell->env);
		perror("execve");
		exit(126);
	}
	
	cmd_path = get_cmd_path(cmd->args[0], shell);
	if (!cmd_path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	
	execve(cmd_path, cmd->args, shell->env);
	/* If execve failed, free cmd_path before exiting */
	perror("execve");
	xfree(cmd_path);
	exit(126);
}