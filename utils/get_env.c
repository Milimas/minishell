/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:14:35 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/24 21:49:45 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_strlen_alnum(char *str)
{
	char	*_str;

	_str = str;
	while (*_str && (ft_isalnum(*_str) || *_str == '_'))
		_str++;
	return (_str - str);
}

char	*get_env_variable(char *name)
{
	t_env	*env;

	env = g_data.env;
	if (!ft_strcmp(name, "?"))
		return (ft_itoa(g_data.exit_status));
	while (env)
	{
		if (!ft_strncmp(name, env->key, ft_strlen_alnum(env->key))
			&& env->value)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}
