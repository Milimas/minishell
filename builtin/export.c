/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 00:39:56 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/26 11:09:02 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ex_ist(char *cmd)
{
	t_env	*env;
	int		plus;

	plus = plus_check(cmd);
	env = g_data.env;
	while (env)
	{
		if (plus == 1 && (!ft_strncmp(env->key, cmd, ft_strlen(env->key))))
			return (1);
		else if (!ft_strncmp(env->key, cmd, ft_strlen(env->key)))
			return (1);
		env = env->next;
	}
	return (0);
}

void	ex_modify(char	*cmd)
{
	t_env	*env;

	env = g_data.env;
	while (env)
	{
		if (!ft_strncmp(ft_strjoin(env->key, "="),
				cmd, ft_strchr(cmd, '=') - cmd +1))
			env->value = ft_strdup(ft_strchr(cmd, '=') + 1);
		else if (!ft_strncmp(env->key, cmd, ft_strchr(cmd, '+') - cmd))
		{
			env->value = ft_strjoin(env->value, ft_strchr(cmd, '=') + 1);
			if (!env->value)
				env->value = ft_strdup(ft_strchr(cmd, '=') + 1);
		}
		env = env->next;
	}
}

static int	isvalid(char *args)
{
	int	i;
	int	plus;

	i = 0;
	plus = 0;
	if (*args == '=' || *args == '+' || (!ft_isalpha(*args) && *args != '_'))
	{
		printf("bash: export: `%s': not a valid identifier\n", args);
		g_data.exit_status = 1;
		return (1);
	}
	while (args[i] && args[i] != '=')
	{
		if ((!ft_isalnum(args[i]) && args[i] != '_' && args[i] != '='
				&& args[i] != '+') || plus_check(args) > 1)
		{
			printf("bash: export: `%s': not a valid identifier\n", args);
			g_data.exit_status = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

void	bsh_export(t_cmd *cmd)
{
	char	**args;
	t_env	*env;

	args = cmd->args + 1;
	env = g_data.env;
	if (!*args)
		exp_tab(env);
	else
	{		
		if (isvalid(*args))
			return ;
		while (*args)
		{
			if (ex_ist(*args) && ft_strchr(*args, '='))
				ex_modify(*args);
			else if (!ex_ist(*args))
				envadd_back(&g_data.env, envnew(*args));
			args++;
		}
	}
}
