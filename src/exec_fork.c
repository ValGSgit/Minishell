/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:50:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/07 12:28:27 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_pipeline(t_cmd *cmd, t_shell *shell)
{
	int		count;
	pid_t	last_pid;
	pid_t	*pids;
	int		fork_result;

	count = count_pipeline_cmds(cmd);
	pids = allocate_pid_array(count);
	cmd->pid_array = pids;
	if (!pids)
		return ;
	ignore_signals();
	fork_result = fork_pipeline_commands(cmd, shell, cmd->pid_array, &last_pid);
	if (fork_result == -1 || fork_result == 1)
	{
		safe_free(pids);
		cmd->pid_array = NULL;
		return ;
	}
	wait_for_pipeline(pids, fork_result, last_pid, shell);
	safe_free(pids);
	cmd->pid_array = NULL;
}

pid_t	*allocate_pid_array(int count)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
		return (NULL);
	ft_bzero(pids, sizeof(pid_t) * count);
	return (pids);
}

int	count_pipeline_cmds(t_cmd *cmd)
{
	int		count;
	t_cmd	*node;

	node = cmd;
	count = 0;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}

void	handle_fork_error(pid_t *pids)
{
	if (pids)
		free(pids);
	perror("fork");
}
