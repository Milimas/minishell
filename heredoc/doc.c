/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 08:23:18 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/23 22:37:23 by rouarrak         ###   ########.fr       */
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

char	*ex_ist2(char *cmd)
{
	t_env	*env;

	env = g_data.env;
	while (env)
	{
		if (!ft_strncmp(env->key, cmd, ft_strlen(env->key) + 1))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	expanding(char *str, int fd, char *limiter)
{
	char	**tab1;
	char	**tab2;
	int		i;
	int		j;
	int		x;

	i = 0;
	if (!str || !ft_strcmp(str, limiter))
		return;
	tab1 = ft_split(str, ' ');
	while (tab1[i])
	{
		x = 0;
		if (i)
			ft_putstr_fd(" ", fd);
		while (tab1[i][x] && tab1[i][x] != '$')
			ft_putchar_fd(tab1[i][x++], fd);
		if (ft_strchr(tab1[i], '$'))
		{
			j = 0;
			tab2 = ft_split(tab1[i], '$');
			while (tab2[j])
			{
				if(j != 0)
					ft_putstr_fd(" ", fd);
				if (ex_ist2(tab2[j]))
					ft_putstr_fd(ex_ist2(tab2[j]), fd);
				else
					ft_putstr_fd("", fd);
				j++;
			}
		}
		i++;
	}
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
				///
				if (!ft_strchr(limiter, '\'') || !ft_strchr(limiter, '\"'))
					expanding(str, fd, limiter);
				else
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
		// close(pipe_hd[0]);
		close(pipe_hd[1]);
		redir = redir->next;
	}
}	