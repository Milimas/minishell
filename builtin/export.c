/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 19:23:49 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/09 00:09:52 by rouarrak         ###   ########.fr       */
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

	args = cmd->args + 1;
	while (*args)
	{
		if (ex_ist(*args))
			ex_modify(*args);
		else
			envadd_back(&g_data.env, envnew(*args));
		args++;
	}
}
