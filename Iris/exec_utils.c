/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iionescu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:13:01 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/01 17:13:03 by iionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
}

void	execve_error(t_cmd *cmd)
{
	char	*cmd_name;

	cmd_name = get_clean_cmd_name(cmd->args[0]);
	print_error_message(cmd_name, ": ", strerror(errno));
	ft_putstr_fd("\n", 2);
	exit(126);
}