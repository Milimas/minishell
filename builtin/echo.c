/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:54:21 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/13 02:45:58 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_n_flag(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] != '-')
		return (0);
	i++;
	if (!arg[i])
		return (0);
	while (arg[i] == 'n')
		i++;
	if (arg[i])
		return (0);
	return (1);
}

void	bsh_echo(t_cmd *cmd)
{
	int		n;
	char	**args;

	n = 1;
	args = cmd->args + 1;
	while (*args && check_n_flag(*args))
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
