/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 04:21:22 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/03/20 20:13:13 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	list_is_empty(t_linkedlist *list)
{
	if (list->head == NULL)
		return (1);
	return (0);
}

t_linkedlist	*list_init(t_linkedlist *list)
{
	list = ft_calloc(sizeof(t_linkedlist), 1);
	if (!list)
		return (NULL);
	return (list);
}

t_elem	*list_new_elem(
	char *content, int len, enum e_token type, enum e_state state)
{
	t_elem	*elem;

	elem = ft_calloc(sizeof(t_elem), 1);
	if (!elem)
		return (NULL);
	elem->content = ft_substr(content, 0, len);
	if (!elem->content)
		return (NULL);
	elem->len = len;
	elem->type = type;
	elem->state = state;
	return (elem);
}

void	list_add_back(t_linkedlist *list, t_elem *new)
{
	if (list_is_empty(list))
		list->head = new;
	else
	{
		list->tail->next = new;
		new->prev = list->tail;
	}
	list->tail = new;
	list->size++;
}

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
