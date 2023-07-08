/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 01:50:20 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/08 03:48:35 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	quote_syntax(t_elem **elem)
{
	enum e_token	quote_type;

	if ((*elem) && is_quote((*elem)) && (*elem)->state == GENERAL)
	{
		quote_type = (*elem)->type;
		(*elem) = (*elem)->next;
		while ((*elem) && is_in_quote((*elem)))
			(*elem) = (*elem)->next;
		if (!(*elem) || (*elem)->type != quote_type)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	logical_syntax(t_elem *elem)
{
	if (elem && elem->state == GENERAL
		&& is_logical_operator(elem))
	{
		elem = elem->next;
		while (elem && elem->type == WHITE_SPACE)
			elem = elem->next;
		if (!elem || is_logical_operator(elem)
			|| !is_type_state(elem, PARENTASIS_CLOSE, GENERAL))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

t_elem	*get_prev_token(t_elem *elem)
{
	if (!elem)
		return (NULL);
	elem = elem->prev;
	while (elem && !is_type_state(elem, WHITE_SPACE, GENERAL))
		elem = elem->prev;
	return (elem);
}

int	check_parentasis(t_elem **elem)
{
	if ((*elem) && !is_type_state((*elem), PARENTASIS_OPEN, GENERAL))
	{
		if (parentasis_syntax((*elem)))
			return (EXIT_FAILURE);
		while ((*elem) && (*elem)->type != PARENTASIS_CLOSE)
			(*elem) = (*elem)->next;
		if ((*elem) && (*elem)->type == PARENTASIS_CLOSE
			&& (*elem)->state == GENERAL)
			(*elem) = (*elem)->next;
	}
	return (EXIT_SUCCESS);
}

int	parentasis_syntax(t_elem *elem)
{
	if (elem->type == PARENTASIS_OPEN && elem->state == GENERAL)
	{
		if (elem->prev && !is_logical_operator(get_prev_token(elem))
			&& get_prev_token(elem)->type != PARENTASIS_OPEN)
			return (EXIT_FAILURE);
		elem = elem->next;
		if ((elem->type == PARENTASIS_CLOSE && elem->state == GENERAL)
			|| is_logical_operator(elem))
			return (EXIT_FAILURE);
		while (elem)
		{
			elem = elem->next;
			if (check_parentasis(&elem))
				return (EXIT_FAILURE);
			if (elem && elem->type == PARENTASIS_CLOSE
				&& elem->state == GENERAL)
				return (EXIT_SUCCESS);
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
