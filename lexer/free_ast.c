/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:48:35 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/03 04:56:59 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_redir(t_redir_list *redir)
{
	t_redir_elem	*elem;
	t_redir_elem	*tmp;

	elem = redir->head;
	while (elem)
	{
		tmp = elem;
		free(elem->arg);
		elem = elem->next;
		free (tmp);
	}
	free(redir);
}

void	free_cmd(t_cmd *cmd)
{
	free_split(cmd->args);
	free_redir(cmd->redir);
	free(cmd);
}

void	free_pipe(t_pipe *pipe)
{
	free_tree(pipe->first);
	free_tree(pipe->second);
	free(pipe);
}

void	free_sub(t_ast_node *sub)
{
	free_tree(sub->content->ast);
}

void	free_tree(t_ast_node *ast)
{
	if (!ast)
		return ;
	if (ast->type == CMD)
		free_cmd(ast->content->cmd);
	if (ast->type == PIPE || ast->type == AND || ast->type == OR)
		free_pipe(ast->content->pipe);
	if (ast->type == SUB)
		free_sub(ast);
	free(ast->content);
	free(ast);
}
