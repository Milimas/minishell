/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sets.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 08:59:12 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/26 10:55:20 by rouarrak         ###   ########.fr       */
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

void	free_env(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
}
