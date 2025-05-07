/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/28 15:40:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_quoted(char *arg, int *i, t_expander_state *state)
{
	if (arg[*i] == '\'' && !state->in_dquote)
	{
		state->in_quote = !state->in_quote;
		(*i)++;
	}
	else if (arg[*i] == '"' && !state->in_quote)
	{
		state->in_dquote = !state->in_dquote;
		(*i)++;
	}
}

static void	process_character(char *arg, int *i, t_expander_state *state,
			t_shell *shell)
{
	char	temp[2];

	if (arg[*i] == '\'' && !state->in_dquote)
		handle_quoted(arg, i, state);
	else if (arg[*i] == '"' && !state->in_quote)
		handle_quoted(arg, i, state);
	else if (arg[*i] == '$' && (!state->in_quote || state->in_dquote))
	{
		state->is_heredoc = false;
		*i = handle_dollar_sign(arg, *i, state, shell);
		(*i)++;
	}
	else
	{
		temp[0] = arg[*i];
		temp[1] = '\0';
		state->result = append_str(state->result, temp);
		if (!state->result)
			state->result = ft_strdup("");
		(*i)++;
	}
}

char	*process_argument(char *arg, t_shell *shell)
{
	t_expander_state	state;
	int					i;

	ft_memset(&state, 0, sizeof(t_expander_state));
	state.result = ft_strdup("");
	if (!state.result || !arg)
		return (state.result);
	i = 0;
	while (arg[i])
		process_character(arg, &i, &state, shell);
	return (state.result);
}

void	handle_special_dir(t_cmd *cmd, t_shell *shell, char *error_msg,
			int exit_code)
{
	ft_putstr_fd(error_msg, 2);
	shell->exit_status = exit_code;
	free(cmd->args[0]);
	free(cmd->args);
	cmd->args = NULL;
	cmd->syntax_error = 1;
}
