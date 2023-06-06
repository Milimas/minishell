/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 19:23:49 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/05 19:41:58 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bsh_export(t_cmd *cmd)
{
	char	**args;

	args = cmd->args + 1;
	while (*args)
	{
		if (ft_strchr(*args, '='))
			envadd_back(&g_data.env, envnew(*args));
		args++;
	}
}
