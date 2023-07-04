/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 03:26:47 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/04 15:15:16 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_args(t_elem *elem)
{
	int	count;

	count = 0;
	while (elem && !(is_logical_operator(elem) && elem->state == GENERAL))
	{
		elem = elem->next;
		count++;
	}
	return (count);
}

char	*creat_arg(t_elem **elem, char *arg)
{
	if ((*elem) && is_quote(*elem))
		arg = ft_strdup("");
	while ((*elem) && is_type_state(*elem, ' ', GENERAL)
		&& !((is_logical_operator(*elem) || is_redirection(*elem))
			&& (*elem)->state == GENERAL))
	{
		while (*elem && is_quote(*elem) && (*elem)->state == GENERAL)
			*elem = (*elem)->next;
		if (!*elem || !is_type_state((*elem), ')', GENERAL)
			|| !is_type_state(*elem, ' ', GENERAL))
			return (arg);
		arg = ft_strconcat(arg, (*elem)->content);
		(*elem) = (*elem)->next;
	}
	return (arg);
}

void	init_cmd(t_cmd *cmd, t_elem **elem)
{
	char	**tmp_args;

	tmp_args = cmd->args;
	while (*elem)
	{
		if ((*elem) && is_redirection(*elem) && (*elem)->state == GENERAL)
			append_redir(cmd->redir, create_redir(elem));
		if (!*elem)
			return ;
		if (is_type_state(*elem, WHITE_SPACE, GENERAL))
		{
			*tmp_args = creat_arg(elem, *tmp_args);
			tmp_args++;
		}
		if ((*elem) && is_logical_operator(*elem) && (*elem)->state == GENERAL)
			break ;
		if ((*elem) && !is_redirection(*elem))
			(*elem) = (*elem)->next;
	}
}

t_ast_node	*create_cmd(t_elem **elem)
{
	t_ast_node	*ast;

	ast = ft_calloc(sizeof(t_ast_node), 1);
	ast->type = CMD;
	ast->content = ft_calloc(sizeof(t_union), 1);
	ast->content->cmd = ft_calloc(sizeof(t_cmd), 1);
	ast->content->cmd->args = ft_calloc(sizeof(char *),
			count_args((*elem)) + 1);
	ast->content->cmd->redir = ft_calloc(sizeof(t_redir_list), 1);
	init_cmd(ast->content->cmd, elem);
	ast->content->cmd->fd.in = 0;
	ast->content->cmd->fd.out = 1;
	return (ast);
}
