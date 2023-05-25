/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:10:20 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 10:21:19 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_redir_elem	*create_redir(t_elem **elem)
{
	t_redir_elem	*redir;

	redir = ft_calloc(sizeof(t_redir_elem), 1);
	redir->type = (*elem)->type;
	*elem = (*elem)->next;
	while ((*elem) && (*elem)->type == WHITE_SPACE)
		*elem = (*elem)->next;
	if ((*elem) && (*elem)->type == WORD)
	{
		redir->arg = "";
		if ((*elem)->state != GENERAL)
		{
			while ((*elem) && (*elem)->state != GENERAL)
			{
				redir->arg = ft_strconcat(redir->arg, (*elem)->content);
				(*elem) = (*elem)->next;
			}
		}
		else
			redir->arg = (*elem)->content;
		if ((*elem))
			(*elem) = (*elem)->next;
	}
	return (redir);
}

void	append_redir(t_redir_list *redir, t_redir_elem *new)
{
	if (!new)
		return ;
	if (!redir->head)
		redir->head = new;
	else
		redir->tail->next = new;
	redir->tail = new;
	redir->size++;
}
