/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 19:57:55 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/01 22:22:43 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bsh_env(int fd)
{
	t_env	*env;

	env = g_data.env;
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->key, fd);
			ft_putchar_fd('=', fd);
			ft_putstr_fd(env->value, fd);
			ft_putchar_fd('\n', fd);
		}
		env = env->next;
	}
}
