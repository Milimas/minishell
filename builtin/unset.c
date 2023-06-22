/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 20:19:39 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/22 16:41:52 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	isvalid(char *args)
{
	int	i;
	int plus;

	i = 0;
	plus = 0;
	if (*args == '=' || *args == '+' || (!ft_isalpha(*args) && *args != '_'))
	{
		printf("bash: unset: `%s': not a valid identifier\n", args);
		return (1);
	}
	while (args[i] && args[i] != '=')
	{
		if ((!ft_isalnum(args[i]) && args[i] != '_' && args[i] != '=' && args[i] != '+') 
		 || plus_check(args) > 1)
		{
			printf("bash: unset: `%s': not a valid identifier\n", args);
			return (1);
		}
		i++;
	}
	return (0);
}

void    bsh_unset(t_cmd *cmd)
{
    char	**args;
    t_env   *env;
    t_env   *hold;
    size_t  len;

    args = cmd->args + 1;
    if (!args && isvalid(*args))
		return ;
    while (*args)
    {
        env = g_data.env;
        len = ft_strlen(*args);
        while(env)
        {
            if (env->next && !ft_strncmp(*args, env->next->key, len))
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
    