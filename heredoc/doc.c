/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 08:23:18 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/22 18:52:56 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*rm_quotes(char *str)
{
	int	nb;
	int	i;
	int	j;
	int len;
	char *res;
	
	nb = 0;
	i = 0;
	res = NULL;
	len = ft_strlen(str);
	while(str[i])
	{
		if (str[i] == '\"')
			nb++;
		i++;
	}
	if (nb % 2 == 0)
	{
		len = len - nb;
		res = malloc( len+1  * sizeof(char));

		i = 0;
		j = 0;
		while (str[i])
		{
			if (j < len && str[i] != '\"')
			{
				res[j] = str[i];
				j++;
			}
			i++;
		}
		res[j] = '\0';
	}
	return (res);
}

void	putfilefd(char *av, int fd)
{
	char	*str;
	char	*limiter;
	
	if (av)
	{
		limiter = rm_quotes(av);
		if (limiter)
		{
			while (1)
			{
				str = readline(">");
				if (!str)
					break ;
				if (ft_strcmp(str, limiter) == 0)
				{
					free (limiter);
					break ;
				}
				ft_putstr_fd(str, fd);
				ft_putstr_fd("\n", fd);
				free(str);
			}		
		}
	}
}

void	rediring(t_redir_elem *redir, t_cmd *cmd)
{
	int pipe_hd[2];
	
	while (redir)
	{
		pipe(pipe_hd);

		if (redir->type == HERE_DOC)
		{	
			putfilefd(redir->arg, pipe_hd[1]);
			close(pipe_hd[1]);
			cmd->fd.in = pipe_hd[0];
		}
		else if (redir->type == REDIRECTION_OUT)
		{
			cmd->fd.out = open(redir->arg, O_CREAT | O_RDWR | O_TRUNC, 0644);
		}
		else if (cmd->redir->tail->type == REDIRECTION_IN)
		{
			cmd->fd.in = open(redir->arg, O_RDONLY, 0644);
			if (cmd->fd.in < 0)
			{
				perror("bash");
				exit (1);
			}
		}
		close(pipe_hd[0]);
		close(pipe_hd[1]);
		redir = redir->next;
	}
}