/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:52:36 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/25 21:15:04 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_error(char *token)
{
	ft_putstr_fd("bash: syntax error near unexpected token ", 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd(token, 2);
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
		if (!elem->next || (is_token(elem->next->type) && !is_quote(elem->next)))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	quote_syntax(t_elem *elem)
{
	enum e_token	quote_type;

	if (elem && is_quote(elem))
	{
		quote_type = elem->type;
		elem = elem->next;
		while (elem && is_in_quote(elem))
			elem = elem->next;
		if (!elem)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_syntax(t_elem *elem)
{
	while (elem && elem->type == WHITE_SPACE)
		elem = elem->next;
	if (is_logical_operator(elem))
		return (syntax_error(elem->content));
	while (elem)
	{
		if (quote_syntax(elem))
			return (syntax_error(elem->content));
		if (redirection_syntax(elem))
			return (syntax_error(elem->content));
		if (elem->type == GENERAL && is_quote(elem) && !elem->next)
			return (syntax_error("newline"));
		if (is_in_quote(elem))
		{
			while (elem && elem->next && elem->next->state != GENERAL)
				elem = elem->next;
			if (!elem->next)
				return (syntax_error("newline"));
			else if (*elem->next->content != (char)elem->state)
				return (syntax_error(elem->next->content));
			elem = elem->next;
		}
		if (elem && elem->state == GENERAL && is_logical_operator(elem) && !elem->next)
			return (syntax_error(elem->content));
		if (elem)
			elem = elem->next;
	}
	return (EXIT_SUCCESS);
}
