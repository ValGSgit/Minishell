/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:33:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 20:47:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	set_default_shlvl(t_shell *shell)
{
	char	*new_shlvl;

	new_shlvl = ft_strdup("SHLVL=1");
	if (new_shlvl)
	{
		update_or_add_env(new_shlvl, &shell->env);
		free(new_shlvl);
	}
}

static void	new_shlvl(t_shell *shell, char *level_str)
{
	char	*newshlvl;

	newshlvl = NULL;
	newshlvl = ft_strjoin("SHLVL=", level_str);
	free(level_str);
	if (new_shlvl)
	{
		update_or_add_env(newshlvl, &shell->env);
		free(newshlvl);
	}
}

void	update_shlvl(t_shell *shell)
{
	int		level;
	char	*level_str;
	char	*val;

	if (!shell->env)
		return ;
	val = get_env_value("SHLVL", shell->env);
	if (!val)
	{
		set_default_shlvl(shell);
		return ;
	}
	level = ft_atoi(val);
	if (level < 0)
		level = 0;
	level++;
	level_str = ft_itoa(level);
	if (!level_str)
		return ;
	new_shlvl(shell, level_str);
}


