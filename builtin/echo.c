/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:54:21 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/02 04:14:55 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bsh_echo(t_cmd *cmd)
{
	int		n;
	char	**args;

	n = 1;
	args = cmd->args + 1;
	if (*args && ft_strnstr(*args, "-n", 3))
	{
		args++;
		n = 0;
	}
	while (*args)
	{
		ft_putstr_fd(*args, cmd->fd.out);
		if (*(args + 1))
			ft_putstr_fd(" ", cmd->fd.out);
		args++;
	}
	write(cmd->fd.out, "\n", 1 * n);
	g_data.exit_status = 0;
}
