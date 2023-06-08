/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 19:57:55 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/08 20:13:58 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    bsh_env()
{
    t_env *env;
    
    env = g_data.env;
    while (env)
    {
        if (ft_strchr(env->value, '='))
            printf("%s\n", env->value);
        env = env->next;
    }
}