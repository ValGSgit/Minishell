/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:59:22 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 23:34:36 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	executor(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
	{
		shell->exit_status = 0;
		return ;
	}
	if (!cmd->next)
		execute_single_command(cmd, shell);
	else
		execute_pipeline(cmd, shell);
}

void	execute_forked_command(t_cmd *cmd, t_shell *shell)
{
	if (cmd->redirs)
	{
		apply_redirection(cmd, true);
	}
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
		free_cmd(cmd);
		exit(shell->exit_status);
	}
	else
	{
		execute_external_command(cmd, shell);
		free_cmd(cmd);
		exit(127);
	}
	free_cmd(cmd);
	exit(shell->exit_status % 256);
}

void	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->args && cmd->args[0] && cmd->args[0][0] == '\0'
		&& !cmd->args[1] && !cmd->redirs)
		return ;
	if (cmd->redirs)
	{
		pid = fork();
		if (pid == 0)
			execute_forked_command(cmd, shell);
		waitpid_for_single_command(pid, shell);
		return ;
	}
	if (cmd->args && cmd->args[0])
	{
		if (is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd);
			return ;
		}
	}
	pid = fork();
	if (pid == 0)
		execute_forked_command(cmd, shell);
	waitpid_for_single_command(pid, shell);
}

void	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	struct stat	sb;
	char		*exec_path;
	char		*cmd_name;

	exec_path = NULL;
	if (cmd->args && cmd->args[0])
	{
		cmd_name = get_clean_cmd_name(cmd->args[0]);
		
		if (cmd->args[0] && shell->path_unset && ft_strchr(cmd->args[0], '/') == NULL)
		{
			print_error_message(cmd_name, ": No such file or directory\n", NULL);
			close_cmd_fds(cmd);
			shell->exit_status = 127;
			exit(127);
		}
		if (access(cmd->args[0], F_OK) != 0)
		{
			if (ft_strchr(cmd->args[0], '/') != NULL)
			{
				print_error_message(cmd_name, ": No such file or directory\n", NULL);
				close_cmd_fds(cmd);
				exit(127);
			}
			else
			{
				print_error_message(cmd_name, ": command not found\n", NULL);
				close_cmd_fds(cmd);
				exit(127);
			}
		}
		if (cmd->args[0] && stat(cmd->args[0], &sb) == 0 && S_ISDIR(sb.st_mode))
		{
			print_error_message(cmd_name, ": Is a directory\n", NULL);
			close_cmd_fds(cmd);
			exit(126);
		}
		if (access(cmd->args[0], X_OK) != 0)
		{
			print_error_message(cmd_name, ": Permission denied\n", NULL);
			close_cmd_fds(cmd);
			exit(126);
		}
		exec_path = ft_strdup(cmd->args[0]);
		if (!exec_path)
		{
			close_cmd_fds(cmd);
			exit(1);
		}
		execve(exec_path, cmd->args, shell->env);
		safe_free(exec_path);
		print_error_message(cmd_name, ": ", strerror(errno));
		ft_putstr_fd("\n", 2);
		close_cmd_fds(cmd);
		exit(126);
	}
	close_cmd_fds(cmd);
	exit(0);
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
	char	*cmd_name;
	
	cmd_name = get_clean_cmd_name(cmd->args[0]);
	if (access(cmd->args[0], F_OK) != 0)
	{
		if (ft_strchr(cmd->args[0], '/') != NULL)
		{
			print_error_message(cmd_name, ": No such file or directory\n", NULL);
			exit(127);
		}
		else
		{
			print_error_message(cmd_name, ": command not found\n", NULL);
			exit(127);
		}
	}
}

void	execve_error(t_cmd *cmd)
{
	char	*cmd_name;
	
	cmd_name = get_clean_cmd_name(cmd->args[0]);
	print_error_message(cmd_name, ": ", strerror(errno));
	ft_putstr_fd("\n", 2);
	exit(126);
}

