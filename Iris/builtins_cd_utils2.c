/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iionescu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:47:55 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/01 15:47:59 by iionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	cd_error(char *path_dup)
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

static void	handle_cd_error(t_cmd *cmd, char *old_pwd, char *path_dup)
{
	cd_error(path_dup);
	if (old_pwd != NULL)
		free(old_pwd);
	cmd->shell->exit_status = 1;
}