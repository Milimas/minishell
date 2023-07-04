/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 05:21:33 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/04 15:50:34 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_int(char *arg)
{
	int	i;
	int	len;

	len = ft_strlen(arg);
	i = 0;
	if (ft_strcmp(arg, "9223372036854775807") > 0)
		return (0);
	if ((arg[0] == '+' || arg[0] == '-') && arg[1])
	{
		if (ft_strcmp(arg, "-9223372036854775808") > 0)
			return (0);
		i++;
	}
	while (i < len)
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	bsh_exit(t_cmd *cmd)
{
	if (!cmd->args[1])
	{
		ft_putstr_fd("exit\n", 1);
		exit (g_data.exit_status);
	}
	else if (cmd->args[2] && is_int(cmd->args[1]))
	{
		ft_putstr_fd("exit\nbash: exit: too many arguments\n", 2);
		g_data.exit_status = 1;
	}
	else if (is_int(cmd->args[1]))
	{
		ft_putstr_fd("exit\n", 1);
		exit(ft_atoi(cmd->args[1]));
	}
	if (!is_int(cmd->args[1]))
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit (255);
	}
}
