/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/11/16 16:18:13 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_signals(void)
{
	g_vars.in_pipe = 1;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	sigint_handlerh(int signum)
{
	g_vars.exit_status = 130;
	(void)signum;
	exit(130);
	g_vars.heredoc_interrupted = 1;
}

void	reset_after_heredoc(void)
{
	if (g_vars.heredoc_interrupted)
	{
		g_vars.heredoc_interrupted = 0;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sigint_handler(int sig)
{
	g_vars.exit_status = 130;
	if (sig == SIGINT)
	{
		if (!g_vars.in_pipe && g_vars.heredoc_interrupted == 0)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}
