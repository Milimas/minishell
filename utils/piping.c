/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 06:08:09 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/11 15:15:42 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_ast_pipe(t_ast_node *ast, int in, int out)
{
	int	pipe_fd[2];

	if (ast && ast->type == CMD)
	{
		ast->content->cmd->fd.in = in;
		ast->content->cmd->fd.out = out;
	}
	else if (ast && ast->type == PIPE)
	{
		if (pipe(pipe_fd))
		{
			perror("bash");
			return ;
		}
		set_ast_pipe(ast->content->pipe->first, in, pipe_fd[1]);
		set_ast_pipe(ast->content->pipe->second, pipe_fd[0], out);
	}
	else if (ast && ast->type != SUB)
	{
		set_ast_pipe(ast->content->pipe->first, in, out);
		set_ast_pipe(ast->content->pipe->second, in, out);
	}
	else if (ast)
		set_ast_pipe(ast->content->ast, in, out);
}

void	close_ast_pipe(t_ast_node *ast, int in, int out)
{
	if (!ast)
		return ;
	if (ast->type == CMD)
	{
		if (ast->content->cmd->fd.in != in
			&& ast->content->cmd->fd.in != STDIN_FILENO)
			close(ast->content->cmd->fd.in);
		if (ast->content->cmd->fd.out != out
			&& ast->content->cmd->fd.out != STDOUT_FILENO)
			close(ast->content->cmd->fd.out);
	}
	else if (ast->type == PIPE)
	{
		close_ast_pipe(ast->content->pipe->first, in, out);
		close_ast_pipe(ast->content->pipe->second, in, out);
	}
	else if (ast->type != SUB)
	{
		close_ast_pipe(ast->content->pipe->first, in, out);
		close_ast_pipe(ast->content->pipe->second, in, out);
	}
	else
		close_ast_pipe(ast->content->ast, in, out);
}
