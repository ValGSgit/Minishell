/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:33:47 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 23:32:45 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int	is_env_cmd(char *cmd)
{
	if (!cmd)
		return (0);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_cmd *cmd)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		ft_cd(cmd);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		ft_pwd(cmd);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		ft_export(cmd);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		ft_unset(cmd);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		ft_env(cmd);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		ft_exit(cmd);
}

/**
 * Creates and prints a single error message to stderr
 * Takes variable parts and joins them before printing
 */
/**
 * Builds an error message from prefix, msg, and extra components
 * Returns a newly allocated string that must be freed by the caller
 */
static char	*build_error_message(const char *prefix, char *msg, char *extra)
{
	char	*error_msg;
	char	*temp;

	if (!prefix && !msg && !extra)
		return (NULL);
	if (prefix)
		error_msg = ft_strdup(prefix);
	else
		error_msg = ft_strdup("");
	if (!error_msg)
		return (NULL);
	if (msg)
	{
		temp = error_msg;
		error_msg = ft_strjoin(error_msg, msg);
		free(temp);
		if (!error_msg)
			return (NULL);
	}
	if (extra)
	{
		temp = error_msg;
		error_msg = ft_strjoin(error_msg, extra);
	}
	return (error_msg);
}

/**
 * Creates and prints a single error message to stderr
 * Takes variable parts and joins them before printing
 */
void	print_error_message(const char *prefix, char *msg, char *extra)
{
	char	*error_msg;

	error_msg = build_error_message(prefix, msg, extra);
	if (error_msg)
	{
		ft_putstr_fd(error_msg, 2);
		free(error_msg);
	}
}
