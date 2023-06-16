/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:51:57 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/16 17:04:08 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	in_env(char *key, char *pwd)
{
	t_env	*env;

	env = g_data.env;
	while(env)
	{
		if (!ft_strncmp(env->key, key, ft_strlen(key)))
		{
			env->value = pwd;
			return(1);
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

void	set_pwd()
{
	t_env	*env;

	env = g_data.env;
	while(env)
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
	if (!dir)
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
	set_oldpwd();
	set_pwd();
}
