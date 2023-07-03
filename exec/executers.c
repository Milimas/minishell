/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executers.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 23:40:27 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/03 09:02:40 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_ast_pipe(t_ast_node *ast_elem)
{
	if (!ast_elem->content->pipe->first || !ast_elem->content->pipe->second)
		return ;
	exec_ast(ast_elem->content->pipe->first, ast_elem->type);
	exec_ast(ast_elem->content->pipe->second, ast_elem->type);
}

void	exec_ast_or(t_ast_node *ast_elem)
{
	if (ast_elem->content->pipe->first)
		exec_ast(ast_elem->content->pipe->first, ast_elem->type);
	if (g_data.exit_status && ast_elem->content->pipe->second)
		exec_ast(ast_elem->content->pipe->second, ast_elem->type);
}

void	exec_ast_and(t_ast_node *ast_elem)
{
	if (ast_elem->content->pipe->first)
		exec_ast(ast_elem->content->pipe->first, ast_elem->type);
	if (!g_data.exit_status && ast_elem->content->pipe->second)
		exec_ast(ast_elem->content->pipe->second, ast_elem->type);
}
