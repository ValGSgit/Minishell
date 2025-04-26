/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:33:20 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 12:58:43 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void remove_env_var(t_cmd *cmd, char *arg)
{
	int i;
	int j;
	size_t arg_len;
	bool is_path;

	i = 0;
	arg_len = ft_strlen(arg);
	is_path = (ft_strcmp(arg, "PATH") == 0);
	if (!cmd->shell->env)
		return;

	// Find the variable to remove
	while (cmd->shell->env[i])
	{
		// Check if the variable name matches and is followed by '=' or is the exact name
		if ((strncmp(cmd->shell->env[i], arg, arg_len) == 0 && 
			(cmd->shell->env[i][arg_len] == '=' || cmd->shell->env[i][arg_len] == '\0')))
		{
			// Free the memory for this environment variable
			free(cmd->shell->env[i]);
			
			// Shift all remaining environment variables down
			j = i;
			while (cmd->shell->env[j + 1])
			{
				cmd->shell->env[j] = cmd->shell->env[j + 1];
				j++;
			}
			cmd->shell->env[j] = NULL; // Ensure null termination
			
			// Mark PATH as unset if needed
			if (is_path)
				cmd->shell->path_unset = true;
				
			return; // Exit after removing the first matching variable
		}
		i++;
	}
}

// static int is_valid_identifier(char *str)
// {
//     int i;
    
//     if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
//         return (0);
    
//     i = 1;
//     while (str[i])
//     {
//         if (!ft_isalnum(str[i]) && str[i] != '_')
//             return (0);
//         i++;
//     }
    
//     return (1);
// }

void ft_unset(t_cmd *cmd)
{
    int i;
    int had_error;

    had_error = 0;
    
    // If only unset with no args, just set exit status to 0 and return
    if (!cmd->args[1])
    {
        cmd->shell->exit_status = 0;
        return;
    }
        
    i = 1;
    while (cmd->args[i])
    {
        // if (!is_valid_identifier(cmd->args[i]))
        // {
        //     ft_putstr_fd("minishell: unset: `", 2);
        //     ft_putstr_fd(cmd->args[i], 2);
        //     ft_putstr_fd("': not a valid identifier\n", 2);
        //     had_error = 1;
        // }
        if (cmd->shell->env)
        {
            // If we're unsetting PATH, first check if it exists
            if (ft_strcmp(cmd->args[i], "PATH") == 0)
            {
                char *path_value = get_env_value("PATH", cmd->shell->env);
                // Only mark PATH as unset if it actually exists
                if (path_value)
                    cmd->shell->path_unset = true;
            }
            
            // Remove the environment variable
            remove_env_var(cmd, cmd->args[i]);
            
            // Special case: If we unset PATH, check if it's in env after unsetting
            // This handles the case where PATH was unset but might be redefined
            if (ft_strcmp(cmd->args[i], "PATH") == 0)
            {
                if (get_env_value("PATH", cmd->shell->env))
                    cmd->shell->path_unset = false;
            }
        }
        i++;
    }
    
    cmd->shell->exit_status = had_error ? 1 : 0;
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
	long long num;
	char *endptr;

	// Print exit message if in interactive mode and not in a pipeline
	if (cmd->shell->is_interactive && !cmd->next)
		ft_putstr_fd("exit\n", 2);

	// No args: exit with previous exit status
	if (!cmd->args[1])
	{
		clean_memory();
		exit(cmd->shell->exit_status);
	}
		
	// Convert argument to number using strtol to detect numeric errors
	errno = 0;
	num = strtol(cmd->args[1], &endptr, 10);
	
	// Check for conversion errors (non-numeric or out of range)
	if (*endptr != '\0' || errno == ERANGE)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		clean_memory();
		exit(2);
	}
	
	// When we get here, we have a valid number
	exit_code = (unsigned char)num; // Use unsigned char to handle proper truncation
	
	// Check for too many arguments
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		cmd->shell->exit_status = 1;
		return; // Don't exit, just set error status
	}
	
	clean_memory();
	exit(exit_code); // Let the OS handle the truncation to 8 bits
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

    if (!cmd->args[1])
    {
        ft_putchar_fd('\n', STDOUT_FILENO);
        cmd->shell->exit_status = 0;
        return;
    }
    // Process all flags at the beginning
    while (cmd->args[i] && cmd->args[i][0] == '-')
    {
        // Check if it's only -n with optional multiple 'n's (like -nnn)
        int j = 1;
        while (cmd->args[i][j] && cmd->args[i][j] == 'n')
            j++;
            
        // If we found anything other than 'n' or reached the end with no 'n's, it's not a valid -n flag
        if (cmd->args[i][j] != '\0' || j == 1)
            break;
        
        n_flag = 1;
        i++;
    }

    // Print the remaining arguments with spaces between them
    while (cmd->args[i])
    {
        ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
        if (cmd->args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }

    // Print newline if -n flag is not set
    if (!n_flag)
        ft_putchar_fd('\n', STDOUT_FILENO);

    cmd->shell->exit_status = 0;
}
