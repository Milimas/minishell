/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:05:28 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/23 20:32:08 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*state_to_text(int state)
{
	if (state == IN_DOUBLE_QUOTE)
		return ("IN_DOUBLE_QUOTE");
	if (state == IN_QUOTE)
		return ("IN_QUOTE");
	if (state == GENERAL)
		return ("GENERAL");
	if (state == IN_PARENTASIS)
		return ("IN_PARENTASIS");
	return (NULL);
}

char	*type_to_text(int type)
{
	if (type == WORD)
		return ("WORD");
	if (type == WHITE_SPACE)
		return ("WHITE_SPACE");
	if (type == NEW_LINE)
		return ("NEW_LINE");
	if (type == QUOTE)
		return ("QUOTE");
	if (type == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	if (type == ENV)
		return ("ENV");
	if (type == PIPE_LINE)
		return ("PIPE_LINE");
	if (type == REDIRECTION_IN)
		return ("REDIRECTION_IN");
	if (type == REDIRECTION_OUT)
		return ("REDIRECTION_OUT");
	if (type == HERE_DOC)
		return ("HERE_DOC");
	if (type == DOUBLE_REDIRECTION_OUT)
		return ("DOUBLE_REDIRECTION_OUT");
	if (type == WILDCARD)
		return ("WILDCARD");
	if (type == PARENTASIS_OPEN)
		return ("PARENTASIS_OPEN");
	if (type == PARENTASIS_CLOSE)
		return ("PARENTASIS_CLOSE");
	if (type == DOUBLE_AMPERSAND)
		return ("DOUBLE_AMPERSAND");
	if (type == LOGICAL_OR_OPERATOR)
		return ("LOGOCAL_OR_OPERATOR");
	return (NULL);
}

char	*ast_type(t_ast_node *ast)
{
	if (ast->type == CMD)
		return ("CMD");
	if (ast->type == PIPE)
		return ("PIPE");
	if (ast->type == AND)
		return ("AND");
	if (ast->type == OR)
		return ("OR");
	if (ast->type == SUB)
		return ("SUB");
	return (NULL);
}

void	print_linkedlist(t_linkedlist *list)
{
	t_elem	*elem;

	elem = list->head;
	printf("=================================================================\n");
	while (elem)
	{
		printf("content: %s\tlen: %d\tstate: %s\ttype: %s\n", elem->content, elem->len, state_to_text(elem->state), type_to_text(elem->type));
		elem = elem->next;
	}
	printf("=================================================================\n");
}

void	print_ast(t_ast_node *ast)
{
	char			**args;
	// t_redir_elem	*redir;

	if (ast && ast->type == CMD && ast->content && ast->content->cmd && ast->content->cmd->args)
	{
		printf("****************************\n");
		args = ast->content->cmd->args;
		printf("cmd: %s\n", *args);
		while (*++args)
		{
			printf("arg: %s\n", *args);
		}
		// if (ast->content->cmd->redir && ast->content->cmd->redir->head)
		// {
		// 	redir = ast->content->cmd->redir->head;
		// 	while (redir)
		// 	{
		// 		printf("redir: %s %d\n", redir->arg, redir->type);
		// 		redir = redir->next;
		// 	}
		// }
		// else
		// {
		// 	printf("no redir\n");
		// }
		printf("fd\nin:  %d\n", ast->content->cmd->fd.in);
		printf("out: %d\n", ast->content->cmd->fd.out);
		printf("****************************\n");
	}
	if (ast && (ast->type == PIPE || ast->type == AND || ast->type == OR))
	{
		printf("%s\n", ast_type(ast));
		print_ast(ast->content->pipe->first);
		print_ast(ast->content->pipe->second);
	}
	if (ast && ast->type == SUB)
	{
		printf("SUBSEHLL\n");
		print_ast(ast->content->ast);
		printf("ENDSUBSHELL\n");
	}
}

// void	print_ast(t_ast_node *ast)
// {
// 	if (ast && ast->type == CMD)
// 		printf("%s ", *ast->content->cmd->args);
// 	printf("%s\n", ast_type(ast));
// 	if (ast && ast->type == SUB)
// 		print_ast(ast->content->ast);
// 	if (ast && (ast->type == PIPE || ast->type == AND || ast->type == OR))
// 	{
// 		printf("first: ");
// 		print_ast(ast->content->pipe->first);
// 		printf("second: ");
// 		print_ast(ast->content->pipe->second);
// 	}
// }
