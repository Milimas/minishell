/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:52:36 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/26 01:49:20 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_error(t_elem *elem)
{
	ft_putstr_fd("bash: syntax error near unexpected token ", 2);
	ft_putstr_fd("`", 2);
	if (elem)
		ft_putstr_fd(elem->content, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	g_data.exit_status = 258;
	return (EXIT_FAILURE);
}

int	redirection_syntax(t_elem *elem)
{
	if (elem && is_redirection(elem))
	{
		while (elem->next && elem->next->type == WHITE_SPACE)
			elem = elem->next;
		if (!elem->next || (is_token(elem->next->type)
				&& !is_quote(elem->next)))
			return (EXIT_FAILURE);
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
		if (!elem)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	parentasis_syntax(t_elem *elem)
{
	if (elem->type == PARENTASIS_OPEN)
	{
		elem = elem->next;
		if (elem->type == PARENTASIS_CLOSE)
			return (EXIT_FAILURE);
		while (elem)
		{
			if (elem->type == PARENTASIS_OPEN)
			{
				if (parentasis_syntax(elem))
					return (EXIT_FAILURE);
				while (elem && elem->type != PARENTASIS_CLOSE)
					elem = elem->next;
				if (elem && elem->type == PARENTASIS_CLOSE)
					elem = elem->next;
			}
			if (elem && elem->type == PARENTASIS_CLOSE)
				return (EXIT_SUCCESS);
			if (elem)
				elem = elem->next;
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_syntax(t_elem *elem)
{
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
		if (elem)
			elem = elem->next;
	}
	return (EXIT_SUCCESS);
}
