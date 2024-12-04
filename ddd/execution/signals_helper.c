/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:04:26 by iabboudi          #+#    #+#             */
/*   Updated: 2024/11/16 16:18:19 by iabboudi         ###   ########.fr       */
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
