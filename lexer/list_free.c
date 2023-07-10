/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 22:04:50 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/01 22:51:23 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_lexer(void)
{
	t_elem	*tmp;
	t_elem	*tmp_f;

	if (!g_data.lexer.head)
		return ;
	tmp = g_data.lexer.head;
	while (tmp)
	{
		tmp_f = tmp;
		tmp = tmp->next;
		free(tmp_f->content);
		free(tmp_f);
	}
}

void	free_split(char **split)
{
	char	**tmp;

	if (!split)
		return ;
	tmp = split;
	while (*split)
		free(*split++);
	free(tmp);
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
