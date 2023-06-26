/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:52:36 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/26 02:38:01 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_type_state(t_elem *elem, enum e_token type, enum e_state state)
{
	if (elem->type == type && elem->state == state)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	check_syntax(t_elem *elem)
{
	int	parentasis_count;

	parentasis_count = 0;
	while (elem && elem->type == WHITE_SPACE)
		elem = elem->next;
	if (is_logical_operator(elem))
		return (syntax_error(elem));
	while (elem)
	{
		if (quote_syntax(&elem))
			return (syntax_error(elem));
		if (redirection_syntax(elem))
			return (syntax_error(elem));
		if (logical_syntax(elem))
			return (syntax_error(elem));
		if (parentasis_syntax(elem))
			return (syntax_error(elem));
		parentasis_count += !is_type_state(elem, PARENTASIS_OPEN, GENERAL);
		parentasis_count -= !is_type_state(elem, PARENTASIS_CLOSE, GENERAL);
		if (elem)
			elem = elem->next;
	}
	if (parentasis_count)
		return (syntax_error(elem));
	return (EXIT_SUCCESS);
}
