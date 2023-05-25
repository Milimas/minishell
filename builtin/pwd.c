/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:56:25 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 10:02:18 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bsh_pwd(void)
{
	char	*buf;

	buf = getcwd(0, 0);
	printf("%s\n", buf);
	free(buf);
}

void	bsh_env(void)
{
	char	**env;

	env = environ;
	while (*env)
	{
		if (ft_strchr(*env, '='))
			printf("%s\n", *env);
		env++;
	}	
}
