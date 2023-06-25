/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:56:25 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/25 08:22:10 by rouarrak         ###   ########.fr       */
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

void	bsh_pwd(void)
{
	char	*buf;

	buf = getcwd(0, 0);
	if (buf == NULL)
		printf("%s\n", fetch_env());
	else
		printf("%s\n", buf);
	free(buf);
}
