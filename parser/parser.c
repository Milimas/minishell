/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:58:02 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/26 00:34:49 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

enum e_node_type	node_type(t_elem *elem)
{
	if (elem->type == DOUBLE_AMPERSAND)
		return (AND);
	if (elem->type == LOGICAL_OR_OPERATOR)
		return (OR);
	if (elem->type == PIPE_LINE)
		return (PIPE);
	return (CMD);
}

t_ast_node	*ft_parser(t_elem **elem, t_ast_node *ast)
{
	t_ast_node	*ast_new;

	// print_ast(ast);
	ast_new = NULL;
	if ((*elem) && (*elem)->type == PARENTASIS_CLOSE)
	{
		(*elem) = (*elem)->next;
		return (ast);
	}
	if (!*elem)
		return (ast);
	ast_new = ft_calloc(sizeof(t_ast_node), 1);
	while ((*elem) && (*elem)->type == WHITE_SPACE && (*elem)->next)
		(*elem) = (*elem)->next;
	if ((*elem) && !is_logical_operator((*elem)) && (*elem)->type != PARENTASIS_OPEN)
	{
		return (create_cmd(elem));
	}
	if ((*elem) && is_logical_operator((*elem)) && (*elem)->state == GENERAL && ast && ast)
	{
		ast_new->type = node_type((*elem));
		(*elem) = (*elem)->next;
		ast_new->content = ft_calloc(sizeof(t_union), 1);
		ast_new->content->pipe = ft_calloc(sizeof(t_pipe), 1);
		ast_new->content->pipe->first = ast;
		if (*elem)
			ast_new->content->pipe->second = ft_parser(elem, ast_new->content->pipe->second);
	}
	if (*elem && (*elem)->type == PARENTASIS_OPEN)
	{
		(*elem) = (*elem)->next;
		ast_new->type = SUB;
		ast_new->content = ft_calloc(sizeof(t_union), 1);
		while (*elem && (*elem)->type != PARENTASIS_CLOSE)
			ast_new->content->ast = ft_parser(elem, ast_new->content->ast);
	}
	return (ast_new);
}

// t_ast_node	*ft_parser(t_elem **elem, t_ast_node **ast)
// {
// 	t_ast_node	*tmp_ast;
	
// 	tmp_ast = NULL;
// 	if (!*elem)
// 		return (*ast);
// 	while (*elem && (*elem)->type == WHITE_SPACE)
// 		(*elem) = (*elem)->next;
// 	printf("%s\n", (*elem)->content);
// 	while (*elem && (*elem)->type == PARENTASIS_OPEN)
// 	{
// 		tmp_ast = ft_calloc(sizeof(t_ast_node), 1);
// 		tmp_ast->type = SUB;
// 		tmp_ast->content = ft_calloc(sizeof(t_union), 1);
// 		tmp_ast->content->ast = ft_calloc(sizeof(t_ast_node), 1);
// 		(*elem) = (*elem)->next;
// 		tmp_ast->content->ast = ft_parser(elem, &(tmp_ast->content->ast));
// 		return (tmp_ast);
// 	}
// 	if ((*elem) && !is_logical_operator(*elem) && (*elem)->type != PIPE_LINE && (*elem)->type != PARENTASIS_OPEN)
// 	{
// 		(*ast) = ft_calloc(sizeof(t_ast_node), 1);
// 		(*ast)->type = CMD;
// 		(*ast)->content = ft_calloc(sizeof(t_union), 1);
// 		(*ast)->content->cmd = create_cmd(elem);
// 		while (*elem && (*elem)->type == WHITE_SPACE)
// 			(*elem) = (*elem)->next;
// 		if ((*elem) && (*elem)->type != PARENTASIS_CLOSE)
// 			(*ast) = ft_parser(elem, &(*ast));
// 		return ((*ast));
// 	}
// 	if ((*elem) && (*elem)->type == PIPE_LINE)
// 	{
// 		*elem = (*elem)->next;
// 		tmp_ast = ft_calloc(sizeof(t_ast_node), 1);
// 		tmp_ast->type = PIPE;
// 		tmp_ast->content = ft_calloc(sizeof(t_union), 1);
// 		tmp_ast->content->pipe = ft_calloc(sizeof(t_pipe), 1);
// 		tmp_ast->content->pipe->first = *ast;
// 		tmp_ast->content->pipe->second = ft_parser(elem, ast);
// 		return (tmp_ast);
// 	}
// 	if ((*elem) && is_logical_operator(*elem))
// 	{
// 		tmp_ast = *ast;
// 		(*ast) = ft_calloc(sizeof(t_ast_node), 1);
// 		(*ast)->type = node_type(*elem);
// 		*elem = (*elem)->next;
// 		(*ast)->content = ft_calloc(sizeof(t_union), 1);
// 		(*ast)->content->pipe = ft_calloc(sizeof(t_pipe), 1);
// 		(*ast)->content->pipe->first = tmp_ast;
// 		(*ast)->content->pipe->second = ft_parser(elem, &(*ast)->content->pipe->second);
// 		return (*ast);
// 	}
// 	return (*ast);
// }

// void	ft_parser(t_elem **elem, t_ast_node **ast)
// {
// 	t_ast_node	*tmp_ast_node;

// 	if (!*elem || (*elem && (*elem)->type == PARENTASIS_CLOSE))
// 	{
// 		return ;
// 	}
// 	tmp_ast_node = *ast;
// 	while ((*elem) && (*elem)->type == WHITE_SPACE && (*elem)->next)
// 		(*elem) = (*elem)->next;
// 	if (!(*ast))
// 		(*ast) = ft_calloc(sizeof(t_ast_node), 1);
// 	if ((*elem) && !is_logical_operator(*elem) && (*elem)->type != PIPE_LINE && (*elem)->type != PARENTASIS_OPEN)
// 	{
// 		(*ast)->type = node_type((*elem));
// 		(*ast)->content = ft_calloc(sizeof(t_union), 1);
// 		(*ast)->content->cmd = create_cmd(&(*elem));
// 		ft_parser(elem, ast);
// 	}
// 	if ((*elem) && (*elem)->type == PIPE_LINE && (*elem)->state == GENERAL && ast && *ast && (*elem)->next)
// 	{
// 		(*ast) = ft_calloc(sizeof(t_ast_node), 1);
// 		(*ast)->type = node_type((*elem));
// 		(*elem) = (*elem)->next;
// 		(*ast)->content = ft_calloc(sizeof(t_union), 1);
// 		(*ast)->content->pipe = ft_calloc(sizeof(t_pipe), 1);
// 		printf("%p\n", (*ast));
// 		(*ast)->content->pipe->first = tmp_ast_node;
// 		(*ast)->content->pipe->second = ft_calloc(sizeof(t_ast_node), 1);
// 		ft_parser(elem, &(*ast)->content->pipe->second);
// 		return ;
// 	}
// 	if ((*elem) && is_logical_operator((*elem)) && (*elem)->state == GENERAL && ast && *ast && (*elem)->next)
// 	{
// 		(*ast) = ft_calloc(sizeof(t_ast_node), 1);
// 		(*ast)->type = node_type((*elem));
// 		(*elem) = (*elem)->next;
// 		(*ast)->content = ft_calloc(sizeof(t_union), 1);
// 		(*ast)->content->pipe = ft_calloc(sizeof(t_pipe), 1);
// 		printf("%p\n", tmp_ast_node);
// 		(*ast)->content->pipe->first = tmp_ast_node;
// 		(*ast)->content->pipe->second = ft_calloc(sizeof(t_ast_node), 1);
// 		ft_parser(elem, &(*ast)->content->pipe->second);
// 		return ;
// 	}
// 	if (*elem && (*elem)->type == PARENTASIS_OPEN)
// 	{
// 		(*ast)->type = SUB;
// 		(*ast)->content = ft_calloc(sizeof(t_union), 1);
// 		(*ast)->content->ast = ft_calloc(sizeof(t_ast_node), 1);
// 		(*elem) = (*elem)->next;
// 		ft_parser(elem, &(*ast)->content->ast);
// 		(*elem) = (*elem)->next;
// 		ft_parser(elem, ast);
// 	}
// }