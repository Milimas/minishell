/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 19:23:49 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/09 00:37:57 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ex_ist(char *cmd)
{
	t_env	*env;

	env = g_data.env;
	while (env)
	{
		if (!ft_strncmp(env->value , cmd, ft_strchr(env->value, '=') - env->value))
			return (1);
		env = env->next;
	}
	return (0);
}

void	ex_modify(char	*cmd)
{
	t_env	*env;

	env = g_data.env;
	while(env)
	{
		if (!ft_strncmp(env->value , cmd, ft_strchr(env->value, '=') - env->value))
			env->value = cmd;
		env = env->next;
	}
}

void	bsh_export(t_cmd *cmd)
{
	char	**args;
	t_env	*env;

	args = cmd->args + 1;
	env = g_data.env;
	if (!*args)
	{
		while (env)
		{
			printf("declare -x %s", env->key);
			if (env->value)
				printf("=\"%s\"", env->value);
			printf("\n");
			env = env->next;
		}
	}
	while (*args)
	{
		if (ex_ist(*args))
			ex_modify(*args);
		else
			envadd_back(&g_data.env, envnew(*args));
		args++;
	}
}
