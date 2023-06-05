/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:51:57 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/05 13:59:16 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bsh_cd(t_cmd *cmd)
{
	char	*dir;
	DIR		*dirp;
	int		i;

	dir = *(++cmd->args);
	if (!dir)
	{
		i = 0;
		while (g_data.env[i])
		{
			if (!ft_strncmp("HOME=", g_data.env[i],5))
			{
				dir = g_data.env[i] + 5;
				break ;
			}
			i++;
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
