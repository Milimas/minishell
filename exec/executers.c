/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executers.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 23:40:27 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/01 23:49:16 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ast_calls(t_ast_node *ast_elem, int *pipe_fd)
{
	if (ast_elem->content->pipe->first->type == PIPE
		|| ast_elem->content->pipe->first->type == OR
		|| ast_elem->content->pipe->first->type == AND)
		if (ast_elem->content->pipe->first->type != SUB)
			ast_elem->content->pipe->first->content->pipe
				->second->content->cmd->fd.out = pipe_fd[1];
	if (ast_elem->content->pipe->first->type == CMD)
		ast_elem->content->pipe->first->content->cmd->fd.out = pipe_fd[1];
	if (ast_elem->content->pipe->second->type == CMD)
		ast_elem->content->pipe->second->content->cmd->fd.in = pipe_fd[0];
}

void	exec_ast_pipe(t_ast_node *ast_elem)
{
	int	pipe_fd[2];

	if (!ast_elem->content->pipe->first || !ast_elem->content->pipe->second)
		return ;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return ;
	}
	g_data.first_pipe = pipe_fd[0];
	if (ast_elem->type == PIPE)
		ast_calls(ast_elem, pipe_fd);
	exec_ast(ast_elem->content->pipe->first, ast_elem->type);
	close(pipe_fd[1]);
	exec_ast(ast_elem->content->pipe->second, ast_elem->type);
	close(pipe_fd[0]);
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
