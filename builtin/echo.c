/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:54:21 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/05 12:26:27 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	args_num(t_cmd *cmd)
{
	int	i;

	i = 0;
	while(cmd->args[i])
		i++;
	return (i);
}

void	bsh_echo(t_cmd *cmd)
{
	int		n;
	char	*flag;
	char	**args;
	int		nb;

	n = 1;
	args = cmd->args + 1;
	nb = args_num(cmd);
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
		ft_putstr_fd(*args, 1);
		if (nb > 2)
		{
			write(1, " ", 1);
			nb--;		
		}
		args++;
	}
	write(1, "\n", 1 * n);
}
