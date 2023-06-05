/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 05:21:33 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/05 09:34:33 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_int(char *arg)
{
	int	i;
	int	len;

	len = ft_strlen(arg);
	i = 0;
	if (arg[0] == '-' && arg[1])
		i++;
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
		printf("exit\n");
		exit (0);
	}
	else if (cmd->args[2])
		printf("bash: exit: too many arguments\n");
	else if (is_int(cmd->args[1]))
	{
		printf("exit\n");
		exit(ft_atoi(cmd->args[1]));
	}
	else
	{
		printf("exit\nbash: exit: %s: numeric argument required\n", cmd->args[1]);
		exit (255);
	}
}
