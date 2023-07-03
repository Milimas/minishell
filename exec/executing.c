/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 23:51:15 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/03 03:10:53 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_ast_other(t_ast_node *ast_elem)
{
	if (ast_elem && ast_elem->type == PIPE)
		exec_ast_pipe(ast_elem);
	else if (ast_elem && ast_elem->type == AND)
		exec_ast_and(ast_elem);
	else if (ast_elem && ast_elem->type == OR)
		exec_ast_or(ast_elem);
}

void	exec_ast(t_ast_node *ast_elem, enum e_node_type parent_type)
{
	if (ast_elem && ast_elem->type == SUB)
		exec_sub(ast_elem);
	else if (ast_elem && ast_elem->type == CMD && ast_elem->content)
	{
		if (is_builts(ast_elem->content->cmd) && parent_type != PIPE)
		{
			builts(ast_elem->content->cmd);
			return ;
		}
		signal(SIGQUIT, sig_quit_handler);
		g_data.pid = fork();
		if (g_data.pid == -1)
		{
			ft_putendl_fd("bash: fork: Resource temporarily unavailable", 2);
			return ;
		}
		if (!g_data.pid)
			exec_cmd(ast_elem);
		if (parent_type != PIPE)
			update_status();
		signal(SIGQUIT, SIG_IGN);
	}
	else
		exec_ast_other(ast_elem);
}
