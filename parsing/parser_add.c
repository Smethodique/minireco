/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_add.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/29 03:54:15 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->name = NULL;
	cmd->args = (char **)malloc(sizeof(char *) * 1024);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args[0] = NULL;
	cmd->redirections = NULL;
	cmd->arg_count = 0;
	cmd->pipe_next = 0;
	cmd->next = NULL;
	return (cmd);
}

void	add_argument(t_command *cmd, char *arg)
{
	if (cmd->arg_count == 0)
	{
		cmd->name = ft_strdup(arg);
	}
	cmd->args[cmd->arg_count++] = ft_strdup(arg);
	cmd->args[cmd->arg_count] = NULL;
}

void	add_redirection(t_command *cmd, int type, char *filename)
{
	t_redirection	*new_red;
	t_redirection	*last;

	new_red = malloc(sizeof(t_redirection));
	if (!new_red)
		return ;
	new_red->type = type;
	new_red->filename = ft_strdup(filename);
	new_red->next = NULL;
	if (!cmd->redirections)
		cmd->redirections = new_red;
	else
	{
		last = cmd->redirections;
		while (last->next)
			last = last->next;
		last->next = new_red;
	}
}

void	add_command(t_command **list, t_command *cmd)
{
	t_command	*current;

	if (!*list)
	{
		*list = cmd;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = cmd;
}
