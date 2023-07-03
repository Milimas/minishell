/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 05:55:12 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/03 05:56:20 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	writing_hd(char *limiter, int fd, char *av)
{
	char	*str;

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
		if (!ft_strchr(av, '\'') && !ft_strchr(av, '\"'))
			expanding(str, fd, limiter);
		else
			ft_putstr_fd(str, fd);
		ft_putstr_fd("\n", fd);
		free(str);
	}
}

void	putfilefd(char *av, int fd)
{
	char	*limiter;

	if (av)
	{
		limiter = rm_quotes(av);
		if (limiter)
			writing_hd(limiter, fd, av);
	}
}
