/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:33:20 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 10:59:34 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void remove_env_var(t_cmd *cmd, char *arg)
{
	int i;

	i = 0;
	size_t arg_len = ft_strlen(arg);
	if (!cmd->shell->env)
		return;
	while (cmd->shell->env[i])
	{
		if (strncmp(cmd->shell->env[i], arg, arg_len) == 0 && cmd->shell->env[i][arg_len] == '=')
		{
			free(cmd->shell->env[i]);
			while (cmd->shell->env[i])
			{
				cmd->shell->env[i] = cmd->shell->env[i + 1];
				i++;
			}
			break;
		}
		i++;
	}
}

void ft_unset(t_cmd *cmd)
{
	int i;

	if (!cmd->args[1] || !cmd->shell->env)
		return;
	i = 1;
	while (cmd->args[i])
	{
		remove_env_var(cmd, cmd->args[i]);
		i++;
	}
}

int ft_isnumber(char *str)
{
	int i;

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

void ft_exit(t_cmd *cmd)
{
	int exit_code;

	if (!cmd->args[1])
		exit(0);
	exit_code = ft_atoi(cmd->args[1]);
	if (cmd->args[1] && cmd->args[2])
	{
		//write(1, "exit\n", 5);
		write(2, "exit: too many arguments\n", 24);
		exit_code = 1;
		exit(1);
	}
	if (cmd->args[1])
	{
		if ((!ft_isnumber(cmd->args[1])) || (ft_strcmp(cmd->args[1], LMAX) == 0 || ft_strcmp(cmd->args[1], LMIN) == 0))
		{
			write(2, " numeric argument required\n", 27);
			exit(2);
		}
		exit_code = ft_atoi(cmd->args[1]);
	}
	free_env(cmd->shell->env);
	exit(exit_code % 256);
}

int is_builtin(char *cmd)
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

void execute_builtin(t_cmd *cmd)
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

void ft_echo(t_cmd *cmd)
{
	int i = 1;
	int n_flag = 0;

	// Process all consecutive -n style flags
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '-' && cmd->args[i][1] == 'n')
		{
			// Check if it's all 'n' characters after the '-'
			int j = 2;
			int all_n = 1;
			while (cmd->args[i][j])
			{
				if (cmd->args[i][j] != 'n')
				{
					all_n = 0;
					break;
				}
				j++;
			}
			if (all_n)
			{
				n_flag = 1;
				i++;
			}
			else
				break;
		}
		else
			break;
	}
	// Print arguments
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1]) // If not the last argument
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	cmd->shell->exit_status = 0;
}
