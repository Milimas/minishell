/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:56:25 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/01 22:18:33 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*fetch_env(void)
{
	t_env	*env;
	char	*res;

	env = g_data.env;
	while (env)
	{
		if (!ft_strncmp(env->key, "PWD", 4))
		{
			res = env->value;
			return (res);
		}
		env = env->next;
	}
	return (NULL);
}

void	bsh_pwd(int fd)
{
	char	*buf;

	buf = getcwd(0, 0);
	if (buf == NULL)
	{	
		ft_putstr_fd(fetch_env(), fd);
		ft_putstr_fd("\n", fd);
	}
	else
	{
		ft_putstr_fd(buf, fd);
		ft_putstr_fd("\n", fd);
	}
	free(buf);
}
