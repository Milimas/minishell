/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:52:36 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/10 02:51:20 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	syntax_error(char *token)
{
	ft_putstr_fd("bash: syntax error near unexpected token ", 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	g_data.exit_status = 258;
}

void	check_syntax(t_elem *elem)
{
	if (elem->type == PIPE_LINE
		|| elem->type == LOGICAL_OR_OPERATOR || elem->type == DOUBLE_AMPERSAND)
		syntax_error(elem->content);
	while (elem)
	{
		if (elem->type == GENERAL && is_quote(elem) && !elem->next)
		{
			syntax_error("newline");
			break ;
		}
		if (is_in_quote(elem))
		{
			while (elem && elem->next && elem->next->state != GENERAL)
				elem = elem->next;
			if (!elem->next)
			{
				syntax_error("newline");
				break ;
			}
			else if (*elem->next->content != (char)elem->state)
			{
				syntax_error(elem->next->content);
				break ;
			}
			elem = elem->next;
		}
		if (elem && elem->state == GENERAL && is_logical_operator(elem) && !elem->next)
			syntax_error(elem->content);
		if (elem)
			elem = elem->next;
	}
}
