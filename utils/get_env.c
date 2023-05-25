/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:14:35 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 07:14:53 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_variable(char *name)
{
	char	**env;

	env = environ;
	while (*env)
	{
		if (!ft_strncmp(name, *env, ft_strlen(name))
			&& (*env)[ft_strlen(name)] == '=')
			return (ft_strchr(*env, '=') + 1);
		env++;
	}
	return (NULL);
}
