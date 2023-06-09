/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:51:57 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/09 00:40:06 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bsh_cd(t_cmd *cmd)
{
	char	*dir;
	DIR		*dirp;
	t_env	*env;

	dir = *(++cmd->args);
	env = g_data.env;
	if (!dir)
	{
		while (env)
		{
			if (!ft_strncmp("HOME=", env->value, 5))
			{
				dir = env->value + 5;
				break ;
			}
			env = env->next;
		}
	}
	dirp = opendir(dir);
	if (!dirp)
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(dir, 2);
		perror(" ");
	}
	else
		closedir(dirp);
	chdir(dir);
}
