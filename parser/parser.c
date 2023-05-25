/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:58:02 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 10:03:27 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_parser(t_elem *elem, t_ast_node **ast)
{
	t_ast_node	*tmp_ast_node;

	while (elem->type == WHITE_SPACE)
		elem = elem->next;
	if (elem && elem->type == WORD)
	{
		if (!(*ast))
		{
			(*ast) = ft_calloc(sizeof(t_ast_node), 1);
		}
		(*ast)->type = CMD;
		(*ast)->content = ft_calloc(sizeof(t_union), 1);
		(*ast)->content->cmd = create_cmd(&elem);
	}
	if (elem && elem->type == PIPE_LINE && elem->state == GENERAL && ast)
	{
		tmp_ast_node = ft_calloc(sizeof(t_ast_node), 1);
		tmp_ast_node->type = PIPE;
		tmp_ast_node->content = ft_calloc(sizeof(t_union), 1);
		tmp_ast_node->content->pipe = ft_calloc(sizeof(t_pipe), 1);
		tmp_ast_node->content->pipe->first = *ast;
		*ast = tmp_ast_node;
		(*ast)->content->pipe->second = ft_calloc(sizeof(t_ast_node), 1);
		ft_parser(elem->next, &(*ast)->content->pipe->second);
	}
}
