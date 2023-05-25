/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:11:32 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 10:12:00 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_args(t_elem *elem)
{
	int	count;

	count = 0;
	while (elem && elem->type != PIPE_LINE)
	{
		elem = elem->next;
		count++;
	}
	return (count);
}

t_cmd	*create_cmd(t_elem **elem)
{
	t_cmd	*cmd;
	char	**tmp_args;

	cmd = ft_calloc(sizeof(t_cmd), 1);
	cmd->args = ft_calloc(sizeof(char *), count_args((*elem)) + 1);
	tmp_args = cmd->args;
	cmd->redir = ft_calloc(sizeof(t_redir_list), 1); // fill this only if there is a redirection else set it to null
	while ((*elem))
	{
		if ((*elem) && ((*elem)->type == REDIRECTION_IN || (*elem)->type == REDIRECTION_OUT || (*elem)->type == HERE_DOC || (*elem)->type == DOUBLE_REDIRECTION_OUT))
			append_redir(cmd->redir, create_redir(elem));
		if ((*elem) && ((*elem)->type != WHITE_SPACE))
			*tmp_args++ = ft_strdup((*elem)->content);
		if ((*elem))
			(*elem) = (*elem)->next;
		if ((*elem) && (*elem)->type == PIPE_LINE && (*elem)->state == GENERAL)
			break;
	}
	cmd->fd.in = 0;
	cmd->fd.out = 1;
	return (cmd);
}
