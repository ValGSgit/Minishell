/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:33:20 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/24 13:36:57 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

/* Must fix echo with -n and keep testing expansions*/
void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	x;
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
		while (cmd->args[i])
		{
			x = 0;
			while (cmd->args[i][x])
			{
				if (cmd->args[i][x] != '\\' || (cmd->args[i][x] == ';'
						&& cmd->args[i][x + 1] == ';'))
					printf("%c", cmd->args[i][x]);
				x++;
			}
			i++;
		}
		i++;
	}
	if (n_flag == 1)
		printf("%%\n");
}

/* This seems to be working, not sure if its missing something lol*/
void	ft_cd(t_cmd *cmd)
{
	// t_shell	*shell;
	if (!cmd->args[1])
	{
		if (chdir(getenv("HOME")) != 0)
			perror("cd");
	}
	else if (cmd->args[2])
	{
		write(2, "cd: too many arguments\n", 23);
		exit(1);
		// shell->exit_status = 1; // Set exit code to 1
	}
	else
	{
		if (chdir(cmd->args[1]) != 0)
		{
			perror("cd");
			exit(1);
			// shell->exit_status = 1; // Set exit code to 1
		}
	}
}

/* Working as it should */
void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
	}
	else
	{
		perror("pwd");
	}
}

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
	return (0);
}

void	ft_sort_strings(char **arr, int count)
{
    int		i;
    int		j;
    char	*temp;

	i = 0;
	j = i + 1;
	while (i < count && arr[i])
	{
		while (j++ < count)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}
// /* Need some more work but its almost there */
// void	ft_export(t_cmd *cmd)
// {
// 	char	**env;
// 	char	*arg;
// 	char	*eq;
// 	int		i;

// 	if (!cmd->args[1]) // No arguments, print current environment
// 	{
// 		ft_env(cmd);
// 		return ;
// 	}
// 	i = 1;
// 	while (cmd->args[i])
// 	{
// 		arg = cmd->args[i];
// 		eq = ft_strchr(arg, '='); // Find '=' in the argument
// 		env = cmd->env;
// 		while (env && *env && !((eq && !ft_strncmp(*env, arg, eq - arg)
// 					&& (*env)[eq - arg] == '=') || (!eq && !ft_strncmp(*env,
// 						arg, ft_strlen(arg)) && (*env)[ft_strlen(arg)] == '=')))
// 			env++;
// 		if (env && *env) // If the variable exists, update it
// 		{
// 			free(*env);
// 			*env = ft_strdup(arg);
// 		}
// 		else if (add_new_env_var(arg, env)) // Add a new variable
// 			return ;
// 		i++;
// 	}
// }

void	ft_unset(t_cmd *cmd)
{
	char	**e;
	char	*arg;
	int		i;
	int		j;

	if (!cmd || !cmd->args)
		return;
	i = 1;
	cmd->exit_status = 0;
	while (cmd->args[i])
	{
		arg = cmd->args[i++];
		e = cmd->env;
		j = 0;
		while (e[j])
		{
			if (!ft_strncmp(e[j], arg, ft_strlen(arg))
				&& e[j][ft_strlen(arg)] == '=')
			{
				free(e[j]);
				while (e[j]) // Shift the remaining variables
				{
					e[j] = e[j + 1];
					j++;
				}
				break ;
			}
			j++;
		}
	}
}

void	ft_env(t_cmd *cmd)
{
	char	**env;

	env = cmd->env;
	if (cmd->args[1])
	{
		write(2, "env: too many arguments\n", 25);
		return ;
	}
	while (env)
	{
		printf("%s\n", *env);
		env++;
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

	exit_code = 0;
	if (cmd->args[1])
	{
		if ((!ft_isnumber(cmd->args[1]) && !cmd->args[2])
		|| (ft_strcmp(cmd->args[1], "9223372036854775808") == 0
		|| ft_strcmp(cmd->args[1], "-9223372036854775809") == 0))
		{
			write(2, "exit: ", 6);
			write(2, cmd->args[1], ft_strlen(cmd->args[1]));
			write(2, "numeric argument required\n", 27);
			cmd->exit_status = 2;
			exit(2);
		}
		exit_code = ft_atoi(cmd->args[1]);
		if (cmd->args[2])
		{
			write(2, "exit: too many arguments\n", 26);
			exit_code = 1;
			return ;
		}
	}
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
		ft_pwd();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		ft_export(cmd);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit(cmd);
}
