/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 22:04:50 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/25 22:06:35 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	list_free(t_linkedlist *list)
{
	t_elem	*tmp;

	tmp = list->head;
	while (tmp)
	{
		free(tmp->content);
		free(tmp);
		tmp = tmp->next;
	}
	free(list);
}

void	delete_from_list(t_linkedlist **list, t_elem *elem)
{
	if (!(*list)->head || !elem)
		return ;
	if ((*list)->head == elem)
		(*list)->head = elem->next;
	if ((*list)->tail == elem)
		(*list)->tail = elem->prev;
	if (elem->next != NULL)
		elem->next->prev = elem->prev;
	if (elem->prev != NULL)
		elem->prev->next = elem->next;
	free(elem->content);
	free(elem);
	elem = NULL;
}
