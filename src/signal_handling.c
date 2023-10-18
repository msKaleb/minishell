/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:07:49 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/10 19:29:31 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
*/

void	on_readline(int signal)
{
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		g_error = 128 + SIGINT;
	}
}

void	on_fd(int signal)
{
	g_error = 128 + signal;
	if (signal == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
	}
}

void	on_background(int signal)
{
	if (signal == SIGINT)
		write(1, "\n", 1);
	else if (signal == SIGQUIT)
		write(1, "Quit (core dumped)\n", 19);
}

void	set_signal_action(int phase)
{
	struct sigaction	sa;
	struct sigaction	sa_quit;

	ft_bzero(&sa, sizeof(sa));
	ft_bzero(&sa_quit, sizeof(sa));
	sa.sa_flags = SA_RESTART;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
	if (phase == ON_FT_READLINE)
		sa.sa_handler = &on_readline;
	else if (phase == ON_BACKGROUND)
	{
		sa.sa_handler = on_background;
		sigaction(SIGQUIT, &sa, NULL);
	}
	else if (phase == ON_FD)
		sa.sa_handler = &on_fd;
	sigaction(SIGINT, &sa, NULL);
}
