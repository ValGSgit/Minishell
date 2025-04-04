/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:35:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/02 17:17:02 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_identifier(char *arg)
{
    int i;

    i = 1;
    if (!ft_isalpha(arg[0]) && arg[0] != '_')
        return (0);
    while (arg[i] && arg[i] != '=')
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int find_env_var(char **env, const char *var)
{
    int i = 0;

    while (env[i])
    {
        size_t var_name_len = strcspn(env[i] + 11, "=");
        if (strncmp(env[i] + 11, var, var_name_len) == 0)
            return i;
        i++;
    }
    return -1;
}

static void	print_env(char **env)
{
    int		i;

    if (!env)
       return ;
    i = 0;
    while (env[i] != NULL)
        printf("declare -x %s\n", env[i++]);
}

void	ft_export(t_cmd *cmd)
{
    int		i;

    if (!cmd->shell->env)
        return;
    if (!cmd->args[1])
    {
        print_env(cmd->shell->env);
        return;
    }
    i = 1;
    while (cmd->args[i])
    {
        if (!is_valid_identifier(cmd->args[i]))
        {
            write(2, "export: not a valid identifier\n", 31);
            cmd->shell->exit_status = 1;
        }
        else
        {
            update_or_add_env(cmd->args[i], &cmd->shell->env);
            cmd->shell->exit_status = 0;
        }
        i++;
    }
}

void update_or_add_env(char *arg, char ***env)
{
    int     index;
    char    *new_var;

    index = find_env_var(*env, arg);
    if (index != -1)
    {
        free((*env)[index]);
        new_var = ft_strdup(arg);
        if (!new_var)
            return;
        (*env)[index] = new_var;
    }
    else 
    {
        new_var = ft_strdup(arg);
        if (!new_var)
            return;
        add_to_env(env, new_var);
    }
}

void add_to_env(char ***env, char *new_var)
{
    int     i;
    char    **new_env;

    i = 0;
    while ((*env)[i])
        i++;
    new_env = malloc((i + 2) * sizeof(char *));
    if (!new_env)
        return;
    while((*env)[i])
    {
        new_env[i] = (*env)[i];
        i++;
    }
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    free(*env);
    *env = new_env;
}

