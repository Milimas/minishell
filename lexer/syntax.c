/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:52:36 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/10 04:17:24 by abeihaqi         ###   ########.fr       */
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

int	check_syntax(t_elem *elem)
{
	if (is_logical_operator(elem))
		return (syntax_error(elem->content));
	while (elem)
	{
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
