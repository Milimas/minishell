/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:52:36 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/08 03:46:47 by abeihaqi         ###   ########.fr       */
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

int	syntax_error(t_elem *elem, int estatus)
{
	ft_putstr_fd("bash: syntax error near unexpected token ", 2);
	ft_putstr_fd("`", 2);
	if (elem && *elem->content)
		ft_putstr_fd(elem->content, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	g_data.exit_status = estatus;
	return (EXIT_FAILURE);
}

int	ambiguous_redirect(t_elem *elem, int estatus)
{
	ft_putstr_fd("bash: ", 2);
	if (elem)
		ft_putstr_fd(elem->content, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	g_data.exit_status = estatus;
	return (EXIT_FAILURE);
}

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
	if (elem && is_logical_operator(elem))
		return (syntax_error(elem, 258));
	while (elem)
	{
		if (quote_syntax(&elem))
			return (syntax_error(elem, 258));
		if (redirection_syntax(elem))
			return (EXIT_FAILURE);
		if (logical_syntax(elem))
			return (syntax_error(elem, 258));
		if (parentasis_syntax(elem))
			return (syntax_error(elem, 258));
		parentasis_count += !is_type_state(elem, PARENTASIS_OPEN, GENERAL);
		parentasis_count -= !is_type_state(elem, PARENTASIS_CLOSE, GENERAL);
		if (elem)
			elem = elem->next;
	}
	if (parentasis_count)
		return (syntax_error(elem, 258));
	return (EXIT_SUCCESS);
}
