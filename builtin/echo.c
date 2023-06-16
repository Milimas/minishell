/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:54:21 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/16 15:08:54 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bsh_echo(t_cmd *cmd)
{
	int		n;
	char	*flag;
	char	**args;

	n = 1;
	args = cmd->args + 1;
	while (*args && ft_strnstr(*args, "-", ft_strlen(*args) + 1))
	{
		flag = *args;
		while (++flag && *flag)
		{
			if (!ft_strchr("neE", *flag))
			{
				flag = NULL;
				break ;
			}
		}
		if (!flag)
			break ;
		args++;
		n = 0;
	}
	while (*args)
	{
		ft_putstr_fd(*args, cmd->fd.out);
		if (*(args + 1))
			ft_putstr_fd(" ",cmd->fd.out);
		args++;
	}
	write(1, "\n", 1 * n);
	g_data.envp = 0;
}
