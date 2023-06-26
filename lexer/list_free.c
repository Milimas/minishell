/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 22:04:50 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/26 08:42:20 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_lexer(void)
{
	t_elem	*tmp;
	t_elem	*tmp_f;

	if (!g_data.lexer.head)
		return ;
	tmp = g_data.lexer.head;
	while (tmp)
	{
		tmp_f = tmp;
		tmp = tmp->next;
		free(tmp_f->content);
		free(tmp_f);
	}
}

void	free_split(char **split)
{
	char	**tmp;

	if (!split)
		return ;
	tmp = split;
	while (*split)
		free(*split++);
	free(tmp);
}

void	free_redir(t_redir_list *redir)
{
	t_redir_elem	*elem;

	elem = redir->head;
	while (elem)
	{
		free(elem->arg);
		elem = elem->next;
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

void	delete_from_list(t_linkedlist **list, t_elem *elem)
{
	if (!(*list)->head || !elem)
		return ;
	if ((*list)->head == elem)
		(*list)->head = elem->next;
	if ((*list)->tail == elem)
		(*list)->tail = elem->prev;
	if (elem->next != NULL)
		elem->next->prev = elem->prev;
	if (elem->prev != NULL)
		elem->prev->next = elem->next;
	free(elem->content);
	free(elem);
	elem = NULL;
}
