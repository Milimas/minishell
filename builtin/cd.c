/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:51:57 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/25 08:19:08 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	in_env(char *key, char *pwd)
{
	t_env	*env;

	env = g_data.env;
	while (env)
	{
		if (!ft_strncmp(env->key, key, ft_strlen(key)))
		{
			env->value = pwd;
			return (1);
		}
		env = env->next;
	}
	return (0);
}

void	set_oldpwd(void)
{
	t_env	*env;
	t_env	*res;
	char	*pwd;

	env = g_data.env;
	while (env)
	{
		if (!ft_strncmp("PWD", env->key, 4))
			pwd = env->value;
		env = env->next;
	}
	if (pwd && !in_env("OLDPWD", pwd))
	{
		res = (t_env *)ft_calloc(sizeof(t_env), 1);
		res->key = "OLDPWD";
		res->value = pwd;
		envadd_back(&g_data.env, res);
	}
}

void	set_pwd(void)
{
	t_env	*env;

	env = g_data.env;
	while (env)
	{
		if (!ft_strncmp("PWD", env->key, 4))
			env->value = getcwd(NULL, 0);
		env = env->next;
	}
}

void	bsh_cd(t_cmd *cmd)
{
	char	*dir;
	DIR		*dirp;
	t_env	*env;

	dir = *(++cmd->args);
	env = g_data.env;
	g_data.exit_status = 0;
	if (dir && !ft_strcmp("-", dir))
	{
		while (env)
		{
			if (!ft_strncmp("OLDPWD", env->key, 7))
			{
				dir = env->value;
				if (dir && !*dir)
				{
					printf("\n");
					return ;
				}
				if (dir)
					bsh_pwd();
				break ;
			}
			env = env->next;
		}
		if (!dir || !ft_strcmp("-", dir))
		{
			ft_putstr_fd("bash: cd: OLDPWD not set\n", 2);
			g_data.exit_status = 1;
			return ;
		}
	}
	else if (!dir)
	{
		while (env)
		{
			if (!ft_strncmp("HOME", env->key, 5))
			{
				dir = env->value;
				break ;
			}
			env = env->next;
		}
		if (!dir)
		{
			ft_putstr_fd("bash: cd: HOME not set\n", 2);
			g_data.exit_status = 1;
			return ;
		}
	}
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
