/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:52:36 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/07 00:16:10 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	syntax_error(char *token)
{
	ft_putstr_fd("bash: syntax error near unexpected token ", 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
}

void	check_syntax(t_elem *elem)
{
	if (elem->type == PIPE_LINE
		|| elem->type == LOGICAL_OR_OPERATOR || elem->type == DOUBLE_AMPERSAND)
		syntax_error(elem->content);
	while (elem)
	{
		elem = elem->next;
	}
}
