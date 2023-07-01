/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tools2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:54:29 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/01 22:56:53 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_redir(t_redir_elem *redir)
{
	if (!redir)
		printf("no redir\n");
	while (redir)
	{
		printf("redir: %s %d\n", redir->arg, redir->type);
		redir = redir->next;
	}
}

void	print_cmd(t_ast_node *ast)
{
	char			**args;

	printf("****************************\n");
	args = ast->content->cmd->args;
	printf("cmd: %s\n", *args);
	while (*++args)
		printf("arg: %s\n", *args);
	print_redir(ast->content->cmd->redir->head);
	printf("fd\nin:  %d\n", ast->content->cmd->fd.in);
	printf("out: %d\n", ast->content->cmd->fd.out);
	printf("****************************\n");
}

void	print_ast(t_ast_node *ast)
{
	if (ast && ast->type == CMD && ast->content
		&& ast->content->cmd && ast->content->cmd->args)
		print_cmd(ast);
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
