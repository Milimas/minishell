/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 23:51:15 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/03 08:54:57 by rouarrak         ###   ########.fr       */
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

int	exec_ast(t_ast_node *ast_elem, enum e_node_type parent_type)
{
	if (ast_elem && ast_elem->type == SUB)
		exec_sub(ast_elem);
	else if (ast_elem && ast_elem->type == CMD && ast_elem->content)
	{
		here_doc(ast_elem->content->cmd->redir->head,
			ast_elem->content->cmd);
		// if (is_builts(ast_elem->content->cmd) && parent_type != PIPE)
		// {
		// 	builts(ast_elem->content->cmd);
		// 	return (1);
		// }
		signal(SIGQUIT, sig_quit_handler);
		g_data.pid = fork();
		if (g_data.pid == -1)
			return (ft_putendl_fd(
					"bash: fork: Resource temporarily unavailable", 2), 1);
		if (!g_data.pid)
			exec_cmd(ast_elem);
		if (parent_type != PIPE)
			update_status();
		signal(SIGQUIT, SIG_IGN);
	}
	else
		exec_ast_other(ast_elem);
	return (1);
}
