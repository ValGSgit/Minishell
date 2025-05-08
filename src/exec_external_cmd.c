/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:21:53 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/07 12:44:37 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	executable_cmd_checks(t_cmd *cmd, char *cmd_name)
{
	if (access(cmd->args[0], X_OK) != 0)
	{
		if (ft_strchr(cmd->args[0], '/') == NULL)
		{
			print_error_message(cmd_name, ": command not found\n", NULL);
			close_cmd_fds(cmd);
			forked_exit(127, cmd);
		}
		else
		{
			print_error_message(cmd_name, ": Permission denied\n", NULL);
			close_cmd_fds(cmd);
			forked_exit(126, cmd);
		}
	}
}

static void	directory_cmd_checks(t_cmd *cmd, char *cmd_name)
{
	struct stat	sb;

	if (cmd->args[0] && stat(cmd->args[0], &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		if (ft_strchr(cmd->args[0], '/') == NULL)
		{
			print_error_message(cmd_name, ": command not found\n", NULL);
			close_cmd_fds(cmd);
			forked_exit(127, cmd);
		}
		else
		{
			print_error_message(cmd_name, ": Is a directory\n", NULL);
			close_cmd_fds(cmd);
			forked_exit(126, cmd);
		}
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
			close_cmd_fds(cmd);
			forked_exit(127, cmd);
		}
		else
		{
			print_error_message(cmd_name, ": command not found\n", NULL);
			close_cmd_fds(cmd);
			forked_exit(127, cmd);
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
		close_cmd_fds(cmd);
		shell->exit_status = 127;
		forked_exit(127, cmd);
	}
	extra_cmd_checks(cmd, cmd_name);
	directory_cmd_checks(cmd, cmd_name);
	executable_cmd_checks(cmd, cmd_name);
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
			close_cmd_fds(cmd);
			forked_exit(1, cmd);
		}
		execve(exec_path, cmd->args, shell->env);
		if (exec_path)
			safe_free(exec_path);
		close_cmd_fds(cmd);
		forked_exit(126, cmd);
	}
	close_cmd_fds(cmd);
	forked_exit(0, cmd);
}
