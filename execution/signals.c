/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/24 03:10:21 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}


void	all_signals(void)
{		
	g_vars.in_pipe = 0;
	signal(SIGINT, sigint_handler);	
	setup_terminal();

	signal(SIGQUIT, SIG_IGN);
}

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
void sigint_handlerh(int signum)
{
	(void)signum;
	g_vars.khbi = dup(0);
	close(0);
	g_vars.heredoc_interrupted = 1;
}

void reset_after_heredoc(void)
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
		else if (g_vars.heredoc_interrupted == 1)
		{
			g_vars.exit_status = 130;
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}