/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 09:51:14 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/26 11:13:27 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	isvalid(char *args)
{
	int	i;
	int	plus;

	i = 0;
	plus = 0;
	if (!*args || *args == '=' || *args == '+'
		|| (!ft_isalpha(*args) && *args != '_'))
	{
		printf("bash: unset: `%s': not a valid identifier\n", args);
		return (1);
	}
	while (args[i])
	{
		if (!(ft_isalnum(args[i]) || args[i] == '_')
			|| args[i] == '=' || args[i] == '+')
		{
			printf("bash: unset: `%s': not a valid identifier\n", args);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	unseting(t_env *env, char **args, t_env *hold)
{
	while (env)
	{
		if (env->next && !ft_strcmp(*args, env->next->key))
		{
			hold = env->next;
			env->next = hold->next;
			free_env(hold);
		}
		env = env->next;
	}
}

void	bsh_unset(t_cmd *cmd)
{
	char	**args;
	t_env	*env;
	t_env	*hold;

	args = cmd->args;
	if (!args)
		return ;
	while (*(++args))
	{
		if (isvalid(*args))
			continue ;
		env = g_data.env;
		if (!ft_strcmp(*args, env->key))
		{
			hold = env;
			g_data.env = env->next;
			free_env(hold);
		}
		else
			unseting(env, args, hold);
	}
}
