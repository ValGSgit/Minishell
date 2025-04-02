/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:19:53 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/01 12:09:41 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../includes/minishell.h"

/* Setup signal handlers */
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_int.sa_mask);
	sigemptyset(&sa_quit.sa_mask);
	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/* Handle SIGINT (Ctrl+C) */
void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/* Handle SIGQUIT (Ctrl+\) */
void	handle_sigquit(int sig)
{
	(void)sig;
	write(1, "\b\b  \b\b", 6);
	rl_on_new_line();
	rl_redisplay();
}

/* Reset signal handlers to default */
void	reset_signals(void)
{
	struct sigaction	sa_default;

	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = 0;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
}

/* Ignore signals (used during child process execution) */
void	ignore_signals(void)
{
	struct sigaction sa_ignore;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, NULL);
	sigaction(SIGQUIT, &sa_ignore, NULL);
}