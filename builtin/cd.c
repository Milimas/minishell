/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:51:57 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/01 22:45:08 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*bsh_toldpwd(t_env *env, char *dir, int fd)
{
	while (env)
	{
		if (!ft_strncmp("OLDPWD", env->key, 7))
		{
			dir = env->value;
			if (dir && !*dir)
			{
				ft_putchar_fd('\n', fd);
				return (NULL);
			}
			if (dir)
				bsh_pwd(fd);
			break ;
		}
		env = env->next;
	}
	if (!dir || !ft_strcmp("-", dir))
	{
		ft_putstr_fd("bash: cd: OLDPWD not set\n", 2);
		g_data.exit_status = 1;
		return (NULL);
	}
	return (dir);
}

char	*bsh_mtcd(t_env *env)
{
	while (env)
	{
		if (!ft_strncmp("HOME", env->key, 5))
		{
			if (!env->value)
			{
				ft_putstr_fd("bash: cd: HOME not set\n", 2);
				g_data.exit_status = 1;
				return (NULL);
			}
			return (env->value);
		}
		env = env->next;
	}
	return (NULL);
}

void	opening_dir(char *dir)
{
	DIR		*dirp;

	dirp = opendir(dir);
	if (!dirp)
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(dir, 2);
		perror(" ");
		g_data.exit_status = 1;
	}
	else
		closedir(dirp);
	chdir(dir);
	set_oldpwd();
	set_pwd();
}

void	bsh_cd(t_cmd *cmd)
{
	char	*dir;
	t_env	*env;

	dir = cmd->args[1];
	env = g_data.env;
	g_data.exit_status = 0;
	if (dir && !ft_strcmp("-", dir))
	{
		dir = bsh_toldpwd(env, dir, cmd->fd.out);
		if (dir == NULL)
			return ;
	}
	else if (!dir || !ft_strcmp("~", dir))
	{
		dir = bsh_mtcd(env);
		if (dir == NULL)
			return ;
	}
	opening_dir(dir);
}
