/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:33:20 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/28 13:20:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	remove_env_var(t_cmd *cmd, char *arg)
{
	int		i;
	int		j;
	size_t	arg_len;
	bool	is_path;

	i = 0;
	arg_len = ft_strlen(arg);
	is_path = (ft_strcmp(arg, "PATH") == 0);
	if (!cmd->shell->env)
		return ;
	while (cmd->shell->env[i])
	{
		if ((strncmp(cmd->shell->env[i], arg, arg_len) == 0 && 
			(cmd->shell->env[i][arg_len] == '=' 
			|| cmd->shell->env[i][arg_len] == '\0')))
		{
			free(cmd->shell->env[i]);
			j = i;
			while (cmd->shell->env[j + 1])
			{
				cmd->shell->env[j] = cmd->shell->env[j + 1];
				j++;
			}
			cmd->shell->env[j] = NULL;
			if (is_path)
				cmd->shell->path_unset = true;
			return ;
		}
		i++;
	}
}

void	ft_unset(t_cmd *cmd)
{
	int		i;
	int		had_error;

	had_error = 0;
	if (!cmd->args[1])
	{
		cmd->shell->exit_status = 0;
		return ;
	}
	i = 1;
	while (cmd->args[i])
	{
		if (cmd->shell->env)
		{
			if (ft_strcmp(cmd->args[i], "PATH") == 0)
				cmd->shell->path_unset = true;
			remove_env_var(cmd, cmd->args[i]);
			if (ft_strcmp(cmd->args[i], "PATH") == 0 && 
				get_env_value("PATH", cmd->shell->env))
				cmd->shell->path_unset = false;
		}
		i++;
	}
	cmd->shell->exit_status = had_error ? 1 : 0;
}

int	ft_isnumber(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_cmd *cmd)
{
	int			exit_code;
	long long	num;
	char		*endptr;

	if (cmd->shell->is_interactive && !cmd->next)
		ft_putstr_fd("exit\n", 2);
	if (!cmd->args[1])
	{
		exit(cmd->shell->exit_status);
	}
	errno = 0;
	num = strtol(cmd->args[1], &endptr, 10);
	if (*endptr != '\0' || errno == ERANGE)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	exit_code = (unsigned char)num;
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		cmd->shell->exit_status = 1;
		return ;
	}
	exit(exit_code);
}

static int	check_command_type(char *cmd, char *cmd_name, int len)
{
	if (ft_strncmp(cmd, cmd_name, len) == 0)
		return (1);
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (check_command_type(cmd, "echo", 5))
		return (1);
	if (check_command_type(cmd, "cd", 3))
		return (1);
	if (check_command_type(cmd, "pwd", 4))
		return (1);
	if (check_command_type(cmd, "export", 7))
		return (1);
	if (check_command_type(cmd, "unset", 6))
		return (1);
	if (check_command_type(cmd, "env", 4))
		return (1);
	if (check_command_type(cmd, "exit", 5))
		return (1);
	return (0);
}

void	execute_builtin(t_cmd *cmd)
{
	if (check_command_type(cmd->args[0], "echo", 5))
		ft_echo(cmd);
	else if (check_command_type(cmd->args[0], "cd", 3))
		ft_cd(cmd);
	else if (check_command_type(cmd->args[0], "pwd", 4))
		ft_pwd(cmd);
	else if (check_command_type(cmd->args[0], "export", 7))
		ft_export(cmd);
	else if (check_command_type(cmd->args[0], "unset", 6))
		ft_unset(cmd);
	else if (check_command_type(cmd->args[0], "env", 4))
		ft_env(cmd);
	else if (check_command_type(cmd->args[0], "exit", 5))
		ft_exit(cmd);
}

void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;
	int	j;

	i = 1;
	n_flag = 0;
	if (!cmd->args[1])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		cmd->shell->exit_status = 0;
		return ;
	}
	while (cmd->args[i] && cmd->args[i][0] == '-')
	{
		j = 1;
		while (cmd->args[i][j] && cmd->args[i][j] == 'n')
			j++;
		while (ft_isspace(cmd->args[i][j]))
			j++;
		if (cmd->args[i][j] != '\0' || j == 1)
			break ;
		n_flag = 1;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	cmd->shell->exit_status = 0;
}
