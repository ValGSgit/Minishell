/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:42:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 18:25:59 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_cd_path(t_cmd *cmd, char **path, char **path_dup,
		char *old_pwd)
{
	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0
		|| ft_strcmp(cmd->args[1], "-") == 0)
	{
		*path_dup = ft_strdup(*path);
		if (!*path_dup)
		{
			free(old_pwd);
			cmd->shell->exit_status = 1;
			return ;
		}
		*path = *path_dup;
	}
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0 && *path)
		ft_putendl_fd(*path, STDOUT_FILENO);
}

void	cd_error(char *path_dup)
{
	char	*error_msg;
	char	*temp;
	char	*with_colon;

	error_msg = ft_strjoin("minishell: cd: ", path_dup);
	if (!error_msg)
		return ;
	temp = error_msg;
	with_colon = ft_strjoin(error_msg, ": ");
	free(temp);
	if (!with_colon)
		return ;
	temp = with_colon;
	error_msg = ft_strjoin(with_colon, strerror(errno));
	free(temp);
	if (!error_msg)
		return ;
	temp = error_msg;
	error_msg = ft_strjoin(error_msg, "\n");
	free(temp);
	if (error_msg)
	{
		ft_putstr_fd(error_msg, 2);
		free(error_msg);
	}
}
