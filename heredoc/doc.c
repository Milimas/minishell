/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 08:23:18 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/04 15:17:14 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_permission(char *arg)
{
	ft_putstr_fd("bash: ", 2);
	perror(arg);
	g_data.exit_status = 1;
	return (0);
}

int	check_hd(t_redir_elem *redir, t_cmd *cmd, int *pipe_hd)
{
	if (redir->type == HERE_DOC)
	{
		if (!check_limiter(redir->arg))
		{
			ft_putstr_fd("bash: unexpected EOF while "
				"looking for matching quote\n", 2);
			g_data.exit_status = 258;
			return (0);
		}
		putfilefd(redir->arg, pipe_hd[1]);
		close(pipe_hd[1]);
		if (cmd->fd.in != STDIN_FILENO)
			close(cmd->fd.in);
		cmd->fd.in = pipe_hd[0];
	}
	return (1);
}

int	checks(t_redir_elem *redir, t_cmd *cmd)
{
	if (redir->type == REDIRECTION_OUT)
	{
		cmd->fd.out = open(redir->arg, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (cmd->fd.out == -1)
			return (check_permission(redir->arg));
	}
	else if (redir->type == DOUBLE_REDIRECTION_OUT)
	{
		cmd->fd.out = open(redir->arg, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (cmd->fd.out == -1)
			return (check_permission(redir->arg));
	}
	else if (redir->type == REDIRECTION_IN)
	{
		cmd->fd.in = open(redir->arg, O_RDONLY, 0644);
		if (cmd->fd.in < 0)
			return (check_permission(redir->arg));
	}
	return (1);
}

int	here_doc(t_redir_elem *redir, t_cmd *cmd)
{
	int	pipe_hd[2];

	while (redir)
	{
		pipe(pipe_hd);
		if (!check_hd(redir, cmd, pipe_hd))
		{
			close(pipe_hd[1]);
			return (0);
		}
		close(pipe_hd[1]);
		redir = redir->next;
	}
	return (1);
}

int	rediring(t_redir_elem *redir, t_cmd *cmd)
{
	while (redir)
	{
		if (!checks(redir, cmd))
			return (0);
		redir = redir->next;
	}
	return (1);
}