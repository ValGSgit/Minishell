/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:35:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 10:24:56 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Add a new variable to the environment
 */
void	add_to_env(char ***env, char *new_var)
{
	int		i;
	char	**new_env;

	if (!new_var)
		return ;
	i = 0;
	while ((*env)[i])
		i++;
	new_env = malloc((i + 2) * sizeof(char *));
	if (!new_env)
		return ;
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

// Helper function for sorting environment variables
static void	swap_strings(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

// Perform bubble sort on environment variables
static void	sort_env(char **env_copy, int env_count)
{
	int	i;
	int	j;
	int	swapped;

	for (i = 0; i < env_count - 1; i++)
	{
		swapped = 0;
		for (j = 0; j < env_count - i - 1; j++)
		{
			if (ft_strcmp(env_copy[j], env_copy[j + 1]) > 0)
			{
				swap_strings(&env_copy[j], &env_copy[j + 1]);
				swapped = 1;
			}
		}
		if (swapped == 0)
			break ;
	}
}

// Print environment variables in the "declare -x" format
static void	print_sorted_env(char **env)
{
	int		i;
	int		env_count;
	char	**env_copy;
	char	*equals_pos;
	int		j;

	if (!env)
		return ;
	// Count environment variables
	env_count = 0;
	while (env[env_count])
		env_count++;
	// Create a copy for sorting
	env_copy = malloc((env_count + 1) * sizeof(char *));
	if (!env_copy)
		return ;
	i = 0;
	while (i < env_count)
	{
		env_copy[i] = ft_strdup(env[i]);
		i++;
	}
	env_copy[i] = NULL;
	// Sort the environment copy
	sort_env(env_copy, env_count);
	// Print the sorted environment
	i = 0;
	while (env_copy[i])
	{
		ft_putstr_fd("declare -x ", 1);
		equals_pos = ft_strchr(env_copy[i], '=');
		if (equals_pos)
		{
			// Print up to equals sign
			j = 0;
			while (env_copy[i][j] != '=')
			{
				ft_putchar_fd(env_copy[i][j], 1);
				j++;
			}
			// Print equals sign and quoted value
			ft_putstr_fd("=\"", 1);
			j++;
			while (env_copy[i][j])
			{
				if (env_copy[i][j] == '\"' || env_copy[i][j] == '$' || 
					env_copy[i][j] == '\\' || env_copy[i][j] == '`')
					ft_putchar_fd('\\', 1);
				ft_putchar_fd(env_copy[i][j], 1);
				j++;
			}
			ft_putstr_fd("\"", 1);
		}
		else
		{
			// No equals sign, just print the name
			ft_putstr_fd(env_copy[i], 1);
		}
		ft_putchar_fd('\n', 1);
		i++;
	}
	// Free the copy
	i = 0;
	while (env_copy[i])
	{
		free(env_copy[i]);
		i++;
	}
	free(env_copy);
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*extract_var_name(char *arg)
{
	char	*equals_pos;
	char	*plus_pos;
	char	*var_name;
	int		name_len;

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
	return (var_name);
}

static char	*handle_append_syntax(char *arg, char **env)
{
	char	*plus_pos;
	char	*equals_pos;
	char	*key;
	char	*existing_value;
	char	*value_to_append;
	char	*new_value;
	char	*new_var;
	int		key_len;
	char	*temp;
	int		j;
	int		k;

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
		temp = new_var;
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
		j = 0;
		k = 0;
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

void	update_or_add_env(char *arg, char ***env)
{
	char	*var_name;
	char	*new_var;
	int		i;
	int		found;
	char	*current_name;

	// Handle append syntax
	if (ft_strchr(arg, '+'))
	{
		new_var = handle_append_syntax(arg, *env);
		if (new_var)
		{
			update_or_add_env(new_var, env);
			free(new_var);
			return ;
		}
	}
	// Extract variable name
	var_name = extract_var_name(arg);
	if (!var_name)
		return ;
	// Find and update existing variable or add new one
	i = 0;
	found = 0;
	while ((*env)[i])
	{
		current_name = extract_var_name((*env)[i]);
		if (current_name && ft_strcmp(current_name, var_name) == 0)
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(arg);
			found = 1;
			free(current_name);
			break ;
		}
		free(current_name);
		i++;
	}
	// Add new variable if not found
	if (!found)
		add_to_env(env, ft_strdup(arg));
	free(var_name);
}

void	ft_export(t_cmd *cmd)
{
	int i;

	// No arguments, just print environment
	if (!cmd->args[1])
	{
		print_sorted_env(cmd->shell->env);
		cmd->shell->exit_status = 0;
		return ;
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