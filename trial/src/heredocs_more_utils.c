/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_more_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:21:46 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 19:55:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	process_arg_loop(char *arg, t_expander_state *state, t_shell *shell)
{
	int		i;
	char	*tmp;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
		{
			i = handle_dollar_sign(arg, i, state, shell);
			i++;
		}
		else
		{
			tmp = state->result;
			state->result = append_str(state->result, (char []){arg[i], '\0'});
			if (!state->result)
			{
				free(tmp);
				break ;
			}
			i++;
		}
	}
}

char	*process_da_bullshit(char *arg, t_shell *shell)
{
	t_expander_state	dabullshit;

	ft_memset(&dabullshit, 0, sizeof(t_expander_state));
	dabullshit.result = ft_strdup("");
	if (!dabullshit.result || !arg)
		return (dabullshit.result);
	process_arg_loop(arg, &dabullshit, shell);
	if (arg)
	{
		free(arg);
		arg = NULL;
	}
	return (dabullshit.result);
}

char	*handle_expanded_line(char *arg, char *expanded_line)
{
	if (ft_strcmp(expanded_line, arg) != 0)
	{
		free(arg);
		return (expanded_line);
	}
	return (arg);
}

bool	check_delimiter_quotes(char *eof)
{
	int		i;
	bool	has_quotes;

	if (!eof || !*eof)
		return (true);
	if ((eof[0] == '\'' && eof[1] == '\'' && !eof[2])
		|| (eof[0] == '"' && eof[1] == '"' && !eof[2]))
		return (false);
	i = 0;
	has_quotes = false;
	while (eof[i])
	{
		if (eof[i] == '\'' || eof[i] == '"')
			has_quotes = true;
		i++;
	}
	return (!has_quotes);
}
