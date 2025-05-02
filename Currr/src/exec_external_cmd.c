/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:21:53 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/02 17:44:29 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	directory_cmd_checks(t_cmd *cmd, char *cmd_name)
{
	struct stat	sb;

	if (cmd->args[0] && stat(cmd->args[0], &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		print_error_message(cmd_name, ": Is a directory\n", NULL);
		close_cmd_fds(cmd);
		cleanup_shell(cmd->shell);
		exit(126);
	}
	if (access(cmd->args[0], X_OK) != 0)
	{
		print_error_message(cmd_name, ": Permission denied\n", NULL);
		close_cmd_fds(cmd);
		cleanup_shell(cmd->shell);
		exit(126);
	}
}

static void	extra_cmd_checks(t_cmd *cmd, char *cmd_name)
{
	if (access(cmd->args[0], F_OK) != 0)
	{
		if (ft_strchr(cmd->args[0], '/') != NULL)
		{
			print_error_message(cmd_name,
				": No such file or directory\n", NULL);
			cleanup_shell(cmd->shell);
			exit(127);
		}
		else
		{
			print_error_message(cmd_name, ": command not found\n", NULL);
			cleanup_shell(cmd->shell);
			exit(127);
		}
	}
}

static void	cmd_checks(t_cmd *cmd, t_shell *shell, char *cmd_name)
{
	if (cmd->args[0] && shell->path_unset
		&& ft_strchr(cmd->args[0], '/') == NULL)
	{
		print_error_message(cmd_name,
			": No such file or directory\n", NULL);
		shell->exit_status = 127;
		cleanup_shell(shell);
		exit(127);
	}
	extra_cmd_checks(cmd, cmd_name);
	directory_cmd_checks(cmd, cmd_name);
}

static void	handle_execve_failure(char *exec_path, t_cmd *cmd, char *cmd_name)
{
	safe_free(exec_path);
	safe_free(cmd_name);
	print_error_message(cmd_name, ": ", strerror(errno));
	cleanup_shell(cmd->shell);
	exit(126);
}

void	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	char		*exec_path;
	char		*cmd_name;

	exec_path = NULL;
	if (cmd->args && cmd->args[0])
	{
		cmd_name = get_clean_cmd_name(cmd->args[0]);
		cmd_checks(cmd, shell, cmd_name);
		signal(SIGINT, SIG_DFL);
		exec_path = ft_strdup(cmd->args[0]);
		if (!exec_path)
		{
			cleanup_shell(cmd->shell);
			exit(1);
		}
		execve(exec_path, cmd->args, shell->env);
		handle_execve_failure(exec_path, cmd, cmd_name);
	}
	cleanup_shell(cmd->shell);
	exit(0);
}
