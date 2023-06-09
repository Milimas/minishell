/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 00:39:56 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/09 02:04:04 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ex_ist(char *cmd)
{
	t_env	*env;

	env = g_data.env;
	while (env)
	{
		if (!ft_strncmp(env->key , cmd, ft_strchr(cmd, '=') - cmd))
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
		if (!ft_strncmp(env->key , cmd, ft_strchr(cmd, '=') - cmd))
			env->value = ft_strchr(cmd, '=') + 1;
		env = env->next;
	}
}

void	isvalid(char *args)
{
	int	i;
	
	i = 1;
	if (!ft_isalpha(args[0]) && args[0] != '_')
	{
		printf("export: `%s': not a valid identifier\n", args);
		exit (1);
	}
	while (args[i])
	{
		if (!ft_isalnum(args[i]) && args[i] != '_' && args[i] != '=' && args[i] != '+')
		{
			printf("export: `%s': not a valid identifier\n", args);
			exit (1);
		}
		i++;
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
	else
	{		
		isvalid(*args);	 
		while (*args)
		{
			if (ex_ist(*args) && ft_strchr(*args,'='))
				ex_modify(*args);
			else if (!ex_ist(*args))
				envadd_back(&g_data.env, envnew(*args));
			args++;
		}
	}
}
