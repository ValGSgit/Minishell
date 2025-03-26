/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:35:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/25 16:10:24 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

static int	is_valid_identifier(char *arg)
{
    int	i;

    if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
        return (0);
    i = 1;
    while (arg[i] && arg[i] != '=')
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_')
            return (write(2, "Not a valid identifier\n", 24), 0);
        i++;
    }
    return (1);
}

static void	print_sorted_env(char **env)
{
    int		i;

    if (!env)
       return ;
    i = 0;
    while (env[i] != NULL)
        printf("declare -x %s\n", env[i++]);
}

static void	update_or_add_env(char *arg, char **env)
{
    char	*key;
    char	*value;
    char	*eq;
    int		i;

    eq = ft_strchr(arg, '=');
	i = 0;
    if (eq)
    {
        key = ft_substr(arg, 0, eq - arg);
        value = ft_strdup(eq + 1);
        while (env[i++])
        {
            if (ft_strncmp(env[i], key, ft_strlen(key) == 0) && env[i][ft_strlen(key)] == '=')
            {
                free(env[i]);
                env[i] = ft_strjoin(key, "=");
                env[i] = ft_strjoin(env[i], value);
                free(key);
                free(value);
                return;
            }
        }
        add_new_env_var(arg, env); // Add new variable if not found
        free(key);
        free(value);
    }
    else if (!add_new_env_var(arg, env)) // Add variable without value
        return;
}

static void	handle_invalid_identifier(char *arg, int *exit_status)
{
	(void)arg;
    write(2, "not a valid identifier\n", 24);
    *exit_status = 1;
	exit(*exit_status);
}

void	ft_export(t_cmd *cmd)
{
    int	i;

    if (!cmd->args[1]) // No arguments, print sorted environment
    {
		print_sorted_env(cmd->env);
        return;
    }
    cmd->exit_status = 0;
    i = 1;
    while (cmd->args[i])
    {
        if (!is_valid_identifier(cmd->args[i]))
            handle_invalid_identifier(cmd->args[i], &cmd->exit_status);
        else
            update_or_add_env(cmd->args[i], cmd->env);
        i++;
    }
}