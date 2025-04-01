/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:35:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/31 14:18:23 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

/*
static void	print_env(char **env)
{
    int		i;

    if (!env)
       return ;
    i = 0;
    while (env[i] != NULL)
        printf("declare -x %s\n", env[i++]);
}

void	update_or_add_env(char *arg, char **env)
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

void	ft_export(t_cmd *cmd)
{
    int		i;

    if (!cmd->env)
        return;
    if (!cmd->args[1])
    {
        print_env(cmd->env);
        return;
    }
    i = 1;
    while (cmd->args[i])
    {
        if (!is_valid_identifier(cmd->args[i]))
        {
            write(2, "export: not a valid identifier\n", 31);
            cmd->exit_status = 1;
        }
        else
        {
            update_or_add_env(cmd->args[i], cmd->env);
            cmd->exit_status = 0;
        }
        i++;
    }
} */

/* Export a variable to the environment */
void ft_export(char **env, const char *key, const char *value)
{
    int i = 0;
    char *new_entry;
    size_t len = strlen(key) + strlen(value) + 2; // +2 for '=' and null terminator
    
    new_entry = malloc(len);
    if (!new_entry)
        return;
    snprintf(new_entry, len, "%s=%s", key, value);
    
    while (env[i])
    {
        if (strncmp(env[i], key, strlen(key)) == 0 && env[i][strlen(key)] == '=')
        {
            free(env[i]);
            env[i] = new_entry;
            return;
        }
        i++;
    }
    env[i] = new_entry;
    env[i + 1] = NULL;
}

/* Unset a variable from the environment */
void ft_unset(char **env, const char *key)
{
    int i = 0;
    size_t key_len = strlen(key);
    
    while (env[i])
    {
        if (strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
        {
            free(env[i]);
            while (env[i + 1])
            {
                env[i] = env[i + 1];
                i++;
            }
            env[i] = NULL;
            return;
        }
        i++;
    }
}

/* Remove null or empty strings from args array */
char **ft_clean_args(char **args)
{
    int i = 0, j = 0;
    int count = 0;
    char **cleaned;
    
    while (args[count])
        count++;
    cleaned = malloc(sizeof(char *) * (count + 1));
    if (!cleaned)
        return NULL;
    while (args[i])
    {
        if (args[i] && *args[i])
            cleaned[j++] = args[i];
        else
            free(args[i]);
        i++;
    }
    cleaned[j] = NULL;
    free(args);
    return cleaned;
}
