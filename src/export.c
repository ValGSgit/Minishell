/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:35:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/08 17:19:45 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Check if a variable name is a valid identifier
 */
// static int	is_valid_identifier(char *arg)
// {
//     int		i;
//     char	*equals_pos;
//     char	*plus_pos;

//     if (!arg || !*arg)
//         return (0);
    
//     equals_pos = ft_strchr(arg, '=');
//     plus_pos = ft_strchr(arg, '+');
    
//     // Check if it starts with a letter or underscore
//     if (!ft_isalpha(arg[0]) && arg[0] != '_')
//         return (0);
    
//     i = 1;
//     // Check until we hit equals sign or plus sign (if it's followed by equals)
//     while (arg[i] && arg[i] != '=' && 
//            !(arg[i] == '+' && arg[i+1] && arg[i+1] == '='))
//     {
//         if (!ft_isalnum(arg[i]) && arg[i] != '_')
//             return (0);
//         i++;
//     }
    
//     // Handle the case where '+' is not followed by '='
//     if (plus_pos && (plus_pos + 1) && *(plus_pos + 1) != '=' && 
//         plus_pos < equals_pos)
//         return (0);
        
//     return (1);
// }

// /**
//  * Add a new variable to the environment
//  */
void	add_to_env(char ***env, char *new_var)
{
    int		i;
    char	**new_env;

    if (!new_var)
        return;
        
    i = 0;
    while ((*env)[i])
        i++;
        
    new_env = malloc((i + 2) * sizeof(char *));
    if (!new_env)
        return;
        
    i = 0;
    while ((*env)[i])
    {
        new_env[i] = (*env)[i];
        i++;
    }
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    
    free(*env);
    *env = new_env;
}

// /**
//  * Print environment variables in the "declare -x" format
//  */
static void	print_sorted_env(char **env)
{
    int		i;
    int		j;
    char	*equals_pos;
    
    if (!env)
        return;
        
    i = 0;
    while (env[i])
    {
        printf("declare -x ");
        equals_pos = ft_strchr(env[i], '=');
        if (equals_pos)
        {
            // Print up to equals sign
            j = 0;
            while (env[i][j] != '=')
            {
                printf("%c", env[i][j]);
                j++;
            }
            // Print equals sign and quoted value
            printf("=\"");
            printf("%s", equals_pos + 1);
            printf("\"");
        }
        else
        {
            // No equals sign, just print the name
            printf("%s", env[i]);
        }
        printf("\n");
        i++;
    }
}

static int is_valid_identifier(char *str)
{
    int i;
    
    if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
        return (0);
    
    i = 1;
    while (str[i] && str[i] != '=' && 
           !(str[i] == '+' && str[i + 1] == '='))
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    
    return (1);
}

static char *extract_var_name(char *arg)
{
    char *equals_pos;
    char *plus_pos;
    char *var_name;
    int name_len;
    
    equals_pos = ft_strchr(arg, '=');
    plus_pos = ft_strchr(arg, '+');
    
    // Determine variable name length
    if (plus_pos && plus_pos[1] == '=')
        name_len = plus_pos - arg;
    else if (equals_pos)
        name_len = equals_pos - arg;
    else
        name_len = ft_strlen(arg);
    
    var_name = ft_substr(arg, 0, name_len);
    return var_name;
}

static char *handle_append_syntax(char *arg, char **env)
{
    char *plus_pos;
    char *equals_pos;
    char *key;
    char *existing_value;
    char *value_to_append;
    char *new_value;
    char *new_var;
    int key_len;
    
    plus_pos = ft_strchr(arg, '+');
    equals_pos = ft_strchr(arg, '=');
    
    if (!plus_pos || !equals_pos || plus_pos + 1 != equals_pos)
        return (NULL);
    
    // Extract key (without the +)
    key_len = plus_pos - arg;
    key = ft_substr(arg, 0, key_len);
    if (!key)
        return (NULL);
    
    // Get existing value
    existing_value = get_env_value(key, env);
    
    // Get value to append (after equals)
    value_to_append = equals_pos + 1;
    
    if (existing_value)
    {
        // Create combined value
        new_value = ft_strjoin(existing_value, value_to_append);
        if (!new_value)
        {
            free(key);
            return (NULL);
        }
        
        // Create final VAR=value string
        new_var = ft_strjoin(key, "=");
        if (!new_var)
        {
            free(key);
            free(new_value);
            return (NULL);
        }
        
        // Join key=value
        char *temp = new_var;
        new_var = ft_strjoin(new_var, new_value);
        free(temp);
        free(new_value);
    }
    else
    {
        // No existing value, just create without the +
        new_var = malloc(ft_strlen(arg));
        if (!new_var)
        {
            free(key);
            return (NULL);
        }
        
        // Copy everything except the '+'
        int j = 0;
        int k = 0;
        while (arg[j])
        {
            if (j != (plus_pos - arg))
                new_var[k++] = arg[j];
            j++;
        }
        new_var[k] = '\0';
    }
    
    free(key);
    return (new_var);
}

void update_or_add_env(char *arg, char ***env)
{
    char *var_name;
    char *new_var;
    int i;
    int found;
    
    // Handle append syntax
    if (ft_strchr(arg, '+'))
    {
        new_var = handle_append_syntax(arg, *env);
        if (new_var)
        {
            update_or_add_env(new_var, env);
            free(new_var);
            return;
        }
    }
    
    // Extract variable name
    var_name = extract_var_name(arg);
    if (!var_name)
        return;
    
    // Find and update existing variable or add new one
    i = 0;
    found = 0;
    while ((*env)[i])
    {
        char *current_name = extract_var_name((*env)[i]);
        if (current_name && ft_strcmp(current_name, var_name) == 0)
        {
            free((*env)[i]);
            (*env)[i] = ft_strdup(arg);
            found = 1;
            free(current_name);
            break;
        }
        free(current_name);
        i++;
    }
    
    // Add new variable if not found
    if (!found)
        add_to_env(env, ft_strdup(arg));
    
    free(var_name);
}

void ft_export(t_cmd *cmd)
{
    int i;
    
    // No arguments, just print environment
    if (!cmd->args[1])
    {
        print_sorted_env(cmd->shell->env);
        cmd->shell->exit_status = 0;
        return;
    }
    
    // Process each argument
    i = 1;
    while (cmd->args[i])
    {
        if (!is_valid_identifier(cmd->args[i]))
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(cmd->args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
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