/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 20:19:39 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/08 23:15:53 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    bsh_unset(t_cmd *cmd)
{
    char	**args;
    t_env   *env;
    t_env   *hold;
    size_t  len;

    args = cmd->args + 1;
    while (*args)
    {
        env = g_data.env;
        len = ft_strlen(*args);
        while(env)
        {
            if (env->next && !ft_strncmp(*args, env->next->value, len))
            {
                hold = env->next;
                env->next = hold->next;
                free(hold);
            }
            env = env->next;
        }
        args++;
    }
}
    