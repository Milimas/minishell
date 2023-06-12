/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:51:07 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/12 10:38:07 by rouarrak         ###   ########.fr       */
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
	
	env = (t_env *)ft_calloc(sizeof(t_env), 1);
	if (!env)
		return (NULL);
	if (ft_strchr(value,'+'))
		env->key = ft_substr(value, 0, ft_strchr(value, '+') - value);
	else
		env->key = ft_substr(value, 0, ft_strchr(value, '=') - value);
	if (ft_strchr(value, '='))
		env->value = ft_substr(value, (ft_strchr(value, '=') - value) + 1, ft_strlen(value));
	env->next = NULL;
	return (env);
}

void	init_global_data(void)
{
	char	**env;

	env = environ;
	g_data.envp = environ;
	while (env && *env)
	{
		envadd_back(&g_data.env, envnew(*env));
		env++;
	}
	g_data.exit_status = -1;
}
