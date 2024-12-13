/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:12:12 by iabboudi          #+#    #+#             */
/*   Updated: 2024/12/12 22:41:21 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	gc_free_memrefs(t_memref *mem_ref)
{
	t_memref	*tmp;

	if (!mem_ref)
		return ;
	while (mem_ref)
	{
		tmp = mem_ref;
		mem_ref = mem_ref->next;
		free(tmp->mem_data);
		tmp->mem_data = NULL;
		free(tmp);
	}
}
