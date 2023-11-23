/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 13:18:01 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:10:42 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstaddback(t_env **lst, t_env *new)
{
	t_env	*t;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	t = *lst;
	while (t->next != NULL)
		t = t->next;
	t->next = new;
}

int	handle_history(t_cmd_hist *h)
{
	printhst_list(h);
	return (1);
}

void	free_history_nodes(t_cmd_hist *h)
{
	t_cmd_hist	*temp;

	while (h != NULL)
	{
		temp = h;
		h = h->next;
		free(temp->history);
		free(temp);
	}
}

void	process_history(t_cmd_hist *command, int *i, t_cmd_hist **h)
{
	if (ft_strlen(command->history))
	{
		add_history(command->history);
		command->history_index = ++(*i);
		command->history_size = ft_strlen(command->history);
		command->next = *h;
		*h = command;
	}
}
