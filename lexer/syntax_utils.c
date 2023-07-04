/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 01:50:20 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/04 11:27:38 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirection_syntax(t_elem *elem)
{
	enum e_token	redir_type;

	if (elem && is_redirection(elem) && elem->state == GENERAL)
	{
		redir_type = elem->type;
		elem = elem->next;
		while (elem && elem->type == WHITE_SPACE)
			elem = elem->next;
		if (!elem || (is_token(elem->type)
				&& !is_quote(elem)) || !*elem->content)
			return (syntax_error(elem, 258));
		else if (elem->type == WILDCARD && redir_type != HERE_DOC)
			return (ambiguous_redirect(elem, 1));
		if (redir_type == HERE_DOC && elem->type == WILDCARD)
			elem->type = WORD;
	}
	return (EXIT_SUCCESS);
}

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
		if (!elem || is_logical_operator(elem))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	parentasis_syntax(t_elem *elem)
{
	if (elem->type == PARENTASIS_OPEN && elem->state == GENERAL)
	{
		elem = elem->next;
		if (elem->type == PARENTASIS_CLOSE && elem->state == GENERAL)
			return (EXIT_FAILURE);
		while (elem)
		{
			elem = elem->next;
			if (elem && elem->type == PARENTASIS_OPEN && elem->state == GENERAL)
			{
				if (parentasis_syntax(elem))
					return (EXIT_FAILURE);
				while (elem && elem->type != PARENTASIS_CLOSE)
					elem = elem->next;
				if (elem && elem->type == PARENTASIS_CLOSE
					&& elem->state == GENERAL)
					elem = elem->next;
			}
			if (elem && elem->type == PARENTASIS_CLOSE
				&& elem->state == GENERAL)
				return (EXIT_SUCCESS);
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
