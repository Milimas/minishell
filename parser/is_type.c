/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_type.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 02:35:50 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/08 03:18:44 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_redirection(t_elem *elem)
{
	if (elem->state != GENERAL)
		return (0);
	return (elem->type == REDIRECTION_IN
		|| elem->type == REDIRECTION_OUT
		|| elem->type == HERE_DOC
		|| elem->type == DOUBLE_REDIRECTION_OUT);
}

int	is_quote(t_elem *elem)
{
	return (elem->type == QUOTE || elem->type == DOUBLE_QUOTE);
}

int	is_in_quote(t_elem *elem)
{
	return (elem->state == IN_QUOTE || elem->state == IN_DOUBLE_QUOTE);
}

int	is_logical_operator(t_elem *elem)
{
	if (!elem || elem->state != GENERAL)
		return (0);
	return (elem->type == PIPE_LINE
		|| elem->type == DOUBLE_AMPERSAND
		|| elem->type == LOGICAL_OR_OPERATOR);
}

int	is_ast_logical(t_ast_node *ast)
{
	return (ast->type == PIPE
		|| ast->type == OR
		|| ast->type == AND);
}
