/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 00:39:56 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/04 13:51:05 by rouarrak         ###   ########.fr       */
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
	char	*egal;

	env = g_data.env;
	while (env)
	{
		egal = ft_strjoin(env->key, "=");
		if (!ft_strncmp(egal,
				cmd, ft_strchr(cmd, '=') - cmd +1))
			{
				free (env->value);
				env->value = ft_strdup(ft_strchr(cmd, '=') + 1);
			}
		else if (!ft_strncmp(env->key, cmd, ft_strchr(cmd, '+') - cmd))
		{
			env->value = ft_strconcat(env->value, ft_strchr(cmd, '=') + 1);
			if (!env->value)
				env->value = ft_strdup(ft_strchr(cmd, '=') + 1);
		}
		env = env->next;
		free (egal);
	}
}

void	built_error(char *args)
{
	ft_putstr_fd("bash: export: `", 2);
	ft_putstr_fd(args, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static int	isvalid(char *args)
{
	int	i;
	int	plus;

	i = 0;
	plus = 0;
	if (*args == '=' || *args == '+' || (!ft_isalpha(*args) && *args != '_'))
	{
		built_error(args);
		g_data.exit_status = 1;
		return (1);
	}
	while (args[i] && args[i] != '=')
	{
		if ((!ft_isalnum(args[i]) && args[i] != '_' && args[i] != '='
				&& args[i] != '+') || plus_check(args) > 1)
		{
			built_error(args);
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
		exp_tab(env, cmd->fd.out);
	else
	{
		while (*args)
		{
			if (isvalid(*args))
			{
				args++;
				continue ;
			}
			if (ex_ist(*args) && ft_strchr(*args, '='))
				ex_modify(*args);
			else if (!ex_ist(*args))
				envadd_back(&g_data.env, envnew(*args));
			args++;
		}
	}
}
