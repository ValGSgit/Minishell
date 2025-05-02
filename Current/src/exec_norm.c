/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:59:22 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 15:43:10 by vagarcia         ###   ########.fr       */
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
		apply_redirection(cmd, true);
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
		exit(cmd->shell->exit_status);
	}
	else if (cmd->args && cmd->args[0])
		execute_external_command(cmd, shell);
	else if (!cmd->args && cmd->redirs)
		;
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
