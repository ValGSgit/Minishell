/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:33:20 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/02 10:34:50 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Helper function (put in separate file) */
int	add_new_env_var(char *arg, char **env)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	if (!env)
		return (1);
	while (env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < count)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[count] = ft_strdup(arg);
	new_env[count + 1] = 0;
	env = new_env;
	//if (env)
	//	free_env(env);
	return (0);
}

static void remove_env_var(t_cmd *cmd, char *arg)
{
    int i;
    
    i = 0;
    if (!cmd->env)
        return;
    while (cmd->env[i] && cmd->env[i + 1])
    {
        if (ft_strncmp(cmd->env[i], arg, ft_strlen(arg)) == 0 && cmd->env[i][ft_strlen(arg)] == '=')
        {
            free(cmd->env[i]);
            while (cmd->env[i])
            {
                cmd->env[i] = cmd->env[i + 1];
                i++;
            }
            break;
        }
        i++;
    }
}

void    ft_unset(t_cmd *cmd)
{
    int i;

    if (!cmd->args[1] || !cmd->env)
        return;
    i = 0;
    while (cmd->env[i])
    {
        if (ft_strcmp(cmd->args[1], cmd->env[i]) == 0)
            remove_env_var(cmd, cmd->args[1]);
        i++;
    }
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
	int	exit_code;

	if (!cmd->args[1])
	exit(0);
	exit_code = ft_atoi(cmd->args[1]);
	if (cmd->args[1] && cmd->args[2])
	{
		write(1, "exit\n", 5);
		write(2, "exit: too many arguments\n", 24);
		exit_code = 1;
		exit(1);
	}
	if (cmd->args[1])
	{
		if ((!ft_isnumber(cmd->args[1])) || (ft_strcmp(cmd->args[1], LMAX) == 0
		|| ft_strcmp(cmd->args[1], LMIN) == 0))
		{
			write(2, " numeric argument required\n", 28);
			exit(2);
		}
		exit_code = ft_atoi(cmd->args[1]);
	}
	free_env(cmd->env);
	exit(exit_code % 256);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		ft_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		ft_export(cmd);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit(cmd);
}

void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (cmd->args[i])
	{
		if (cmd->args[i])
			printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
}
