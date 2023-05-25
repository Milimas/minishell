/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:55:11 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 06:56:09 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bsh_chdir(t_cmd *cmd)
{
	char	*dir;
	DIR		*dirp;

	dir = *(++cmd->args);
	if (!cmd->args)
	{
		chdir(NULL);
		return ;
	}
	dirp = opendir(*cmd->args);
	if (!dirp)
		perror("bash: cd");
	else
		closedir(dirp);
	chdir(dir);
}
