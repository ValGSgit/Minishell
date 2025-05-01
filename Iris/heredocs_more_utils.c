/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_more_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:21:46 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 11:27:24 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// extern volatile sig_atomic_t	g_signal_received;

// char	*process_da_bullshit(char *arg, t_shell *shell)
// {
// 	t_expander_state	dabullshit;
// 	int					i;
// 	char				*result;

// 	ft_memset(&dabullshit, 0, sizeof(t_expander_state));
// 	dabullshit.result = ft_strdup("");
// 	if (!dabullshit.result || !arg)
// 		return (dabullshit.result);
// 	i = 0;
// 	while (arg[i])
// 	{
// 		if (arg[i] == '$')
// 			i = handle_dollar_sign(arg, i, &dabullshit, shell);
// 		else
// 		{
// 			result = dabullshit.result;
// 			dabullshit.result = append_str(dabullshit.result, 
// 				(char[]){arg[i], '\0'});
// 			if (result != dabullshit.result && !dabullshit.result)
// 				dabullshit.result = result;
// 		}
// 		i++;
// 	}
// 	return (dabullshit.result);
// }

// static void	process_heredoc_line(char *line, char *processed_line, 
// 	int fd, bool expand_vars)
// {
// 	if (processed_line)
// 	{
// 		write(fd, processed_line, ft_strlen(processed_line));
// 		write(fd, "\n", 1);
// 		free(processed_line);
// 	}
// }

// void	read_heredoc_input(const char *delimiter, int fd, t_cmd *cmd, 
// 	bool expand_vars)
// {
// 	char	*line;
// 	char	*processed_line;

// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `", 2);
// 			ft_putstr_fd((char *)delimiter, 2);
// 			ft_putstr_fd("')\n", 2);
// 			break;
// 		}
// 		if (ft_strcmp(line, delimiter) == 0 && 
// 			ft_strlen(line) == ft_strlen(delimiter))
// 		{
// 			free(line);
// 			break;
// 		}
// 		processed_line = expand_vars ? 
// 			process_da_bullshit(line, cmd->shell) : line;
// 		if (expand_vars)
// 			free(line);
// 		process_heredoc_line(line, processed_line, fd, expand_vars);
// 	}
// }

// void	heredoc_child_process(t_cmd *cmd, char *clean_eof, int fd, bool expand_vars)
// {
// 	signal(SIGINT, SIG_DFL);
// 	setup_heredoc_signals();
// 	read_heredoc_input(clean_eof, fd, cmd, expand_vars);
// 	close(fd);
// 	free(clean_eof);
// 	if (cmd && cmd->shell)
// 	{
// 		rl_clear_history();
// 		cleanup_shell(cmd->shell);
// 	}
// 	exit(0);
// }

// int	heredoc_parent_process(pid_t pid, t_cmd *cmd, int fd, char *clean_eof)
// {
// 	int	status;

// 	close(fd);
// 	free(clean_eof);
// 	signal(SIGINT, SIG_IGN);
// 	waitpid(pid, &status, 0);
// 	setup_signals();
// 	if (WIFSIGNALED(status))
// 	{
// 		cmd->shell->exit_status = 128 + WTERMSIG(status);
// 		g_signal_received = cmd->shell->exit_status;
// 		cleanup_heredocs(cmd->shell);
// 		return (1);
// 	}
// 	else if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
// 	{
// 		cmd->shell->exit_status = 130;
// 		g_signal_received = 130;
// 		cleanup_heredocs(cmd->shell);
// 		return (1);
// 	}
// 	return (0);
// }

// char	*handle_expanded_line(char *arg, char *expanded_line)
// {
// 	if (ft_strcmp(expanded_line, arg) != 0)
// 	{
// 		free(arg);
// 		return (expanded_line);
// 	}
// 	return (arg);
// }

// bool	check_delimiter_quotes(char *eof)
// {
// 	int		i;
// 	bool	has_quotes;

// 	if (!eof || !*eof)
// 		return (true);
// 	if ((eof[0] == '\'' && eof[1] == '\'' && !eof[2]) || 
// 		(eof[0] == '"' && eof[1] == '"' && !eof[2]))
// 		return (false);
// 	i = 0;
// 	has_quotes = false;
// 	while (eof[i])
// 	{
// 		if (eof[i] == '\'' || eof[i] == '"')
// 			has_quotes = true;
// 		i++;
// 	}
// 	return (!has_quotes);
// }

char *process_da_bullshit(char *arg, t_shell *shell)
{
	t_expander_state	dabullshit;
	int				i;
	char			*result;

	ft_memset(&dabullshit, 0, sizeof(t_expander_state));
	dabullshit.result = ft_strdup("");
	if (!dabullshit.result || !arg)
		return (dabullshit.result);
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
		{
			i = handle_dollar_sign(arg, i, &dabullshit, shell);
			i++;
		}
		else
		{
			result = dabullshit.result;
			dabullshit.result = append_str(dabullshit.result, (char[]){arg[i], '\0'});
			if (!dabullshit.result)
			{
				free(result); // or handle cleanup if needed
				break;
			}
			(i)++;
		}
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
	if ((eof[0] == '\'' && eof[1] == '\'' && !eof[2]) || 
		(eof[0] == '"' && eof[1] == '"' && !eof[2]))
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