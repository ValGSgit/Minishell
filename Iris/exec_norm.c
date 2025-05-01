/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:59:22 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 13:13:26 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



void	execute_forked_command(t_cmd *cmd, t_shell *shell)
{
	if (cmd->redirs)
		apply_redirection(cmd, true);
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
		exit(cmd->shell->exit_status);
	}
	else if (cmd->args && cmd->args[0])
		execute_external_command(cmd, shell);
	exit(cmd->exit_status % 256);
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
		if (is_env_cmd(cmd->args[0]) || is_builtin(cmd->args[0]))
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

static void	print_execve_error(char *cmd_name)
{
	print_error_message(cmd_name, ": ", strerror(errno));
	ft_putstr_fd("\n", 2);
	exit(126);
}

void	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	char		*exec_path;
	char		*cmd_name;

	exec_path = NULL;
	cmd_name = NULL;
	if (cmd->args && cmd->args[0])
	{
		if (cmd->args[0])
			cmd_name = get_clean_cmd_name(cmd->args[0]);
		else
			cmd_name = cmd->args[0];
		if (cmd->args[0] && shell->path_unset
			&& ft_strchr(cmd->args[0], '/') == NULL)
		{
			print_error_message(cmd_name,
				": No such file or directory\n", NULL);
			shell->exit_status = 127;
			exit(127);
		}
		exec_path = ft_strdup(cmd->args[0]);
		if (!exec_path)
			exit(1);
		execve(exec_path, cmd->args, shell->env);
		safe_free(exec_path);
		print_execve_error(cmd_name);
	}
	exit(0);
}



static void	directory_sb_check(t_cmd *cmd, char *cmd_name)
{
	struct stat	sb;

	if (cmd->args[0] && stat(cmd->args[0], &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		print_error_message(cmd_name, ": Is a directory\n", NULL);
		exit(126);
	}
}

void	cmd_checks(t_cmd *cmd, char *cmd_name)
{
	if (access(cmd->args[0], F_OK) != 0)
	{
		if (ft_strchr(cmd->args[0], '/') != NULL)
		{
			print_error_message(cmd_name,
				": No such file or directory\n", NULL);
			exit(127);
		}
		else
		{
			print_error_message(cmd_name, ": command not found\n", NULL);
			exit(127);
		}
	}
	directory_sb_check(cmd, cmd_name);
	if (access(cmd->args[0], X_OK) != 0)
	{
		print_error_message(cmd_name, ": Permission denied\n", NULL);
		exit(126);
	}
}
