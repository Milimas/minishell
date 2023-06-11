/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:11:32 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/11 06:38:23 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_args(t_elem *elem)
{
	int	count;

	count = 0;
	while (elem && !is_logical_operator(elem))
	{
		elem = elem->next;
		count++;
	}
	return (count);
}

void	init_cmd(t_cmd *cmd, t_elem **elem)
{
	char	**tmp_args;

	tmp_args = cmd->args;
	while (*elem)
	{
		if ((*elem) && is_redirection(*elem) && (*elem)->state == GENERAL)
			append_redir(cmd->redir, create_redir(elem));
		else if ((*elem))
		{
			if (!((*elem)->type == WHITE_SPACE && (*elem)->state == GENERAL))
			{
				while ((*elem) && !(((*elem)->type == WHITE_SPACE || is_logical_operator(*elem)) && (*elem)->state == GENERAL))
				{
					while (*elem && is_quote(*elem) && (*elem)->state == GENERAL)
						*elem = (*elem)->next;
					if (!*elem)
						break ;
					*tmp_args = ft_strconcat(*(tmp_args), (*elem)->content);
					(*elem) = (*elem)->next;
				}
				tmp_args++;
			}
		}
		if ((*elem) && is_logical_operator(*elem) && (*elem)->state == GENERAL)
			break ;
		if ((*elem))
			(*elem) = (*elem)->next;
	}
}

t_cmd	*create_cmd(t_elem **elem)
{
	t_cmd	*cmd;

	cmd = ft_calloc(sizeof(t_cmd), 1);
	cmd->args = ft_calloc(sizeof(char *), count_args((*elem)) + 1);
	cmd->redir = ft_calloc(sizeof(t_redir_list), 1);
	init_cmd(cmd, elem);
	cmd->fd.in = 0;
	cmd->fd.out = 1;
	return (cmd);
}
