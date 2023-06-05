/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:51:07 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/05 14:29:43 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*envlast(t_env *env)
{
	if (!env)
		return (env);
	while (env->next != NULL)
		env = env->next;
	return (env);
}

void	envadd_back(t_env **env, t_env *new)
{
	if (*env == NULL)
		*env = new;
	else
		envlast(*env)->next = new;
}

t_env	*envnew(char *value)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->value = value;
	env->next = NULL;
	return (env);
}

void	init_global_data(void)
{
	char	**env;

	env = environ;
	while (env && *env)
	{
		envadd_back(&g_data.env, envnew(*env));
	}
	g_data.exit_status = -1;
}
