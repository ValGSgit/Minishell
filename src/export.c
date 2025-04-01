/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:35:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/01 13:45:42 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static void	print_env(char **env)
{
    int		i;

    if (!env)
       return ;
    i = 0;
    while (env[i] != NULL)
        printf("declare -x %s\n", env[i++]);
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
}
