/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executers.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 23:40:27 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/11 09:09:19 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_ast_pipe(t_ast_node *ast_elem)
{
	if (!ast_elem->content->pipe->first || !ast_elem->content->pipe->second)
		return ;
	exec_ast(ast_elem->content->pipe->first, ast_elem->type);
	close_ast_pipe(ast_elem->content->pipe->first, STDIN_FILENO, STDOUT_FILENO);
	exec_ast(ast_elem->content->pipe->second, ast_elem->type);
	close_ast_pipe(ast_elem->content->pipe->second, STDIN_FILENO,
		STDOUT_FILENO);
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

void	exec_sub(t_ast_node *ast_elem)
{
	g_data.subpid = fork();
	if (g_data.subpid == -1)
	{
		ft_putendl_fd("bash: fork: Resource temporarily unavailable", 2);
		return ;
	}
	if (g_data.subpid)
		close_ast_pipe(ast_elem->content->ast, STDIN_FILENO, STDOUT_FILENO);
	if (!g_data.subpid)
	{
		exec_ast(ast_elem->content->ast, ast_elem->content->ast->type);
		close_ast_pipe(g_data.ast.root, STDIN_FILENO, STDOUT_FILENO);
		wait_last();
		exit(g_data.exit_status);
	}
	update_status(g_data.subpid);
}
