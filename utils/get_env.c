/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:14:35 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/10 02:59:58 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_variable(char *name)
{
	t_env	*env;

	env = g_data.env;
	if (!ft_strncmp(name, "?", ft_strlen(name)))
		return (ft_itoa(g_data.exit_status));
	while (env)
	{
		if (!ft_strncmp(name, env->key, ft_strlen(env->key)))
			return (env->value);
		env = env->next;
	}
	return (name);
}
