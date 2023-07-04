/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:51:07 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/04 23:01:04 by rouarrak         ###   ########.fr       */
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
	if (ft_strchr(value, '+'))
		env->key = ft_substr(value, 0, ft_strchr(value, '+') - value);
	else
		env->key = ft_substr(value, 0, ft_strchr(value, '=') - value);
	if (ft_strchr(value, '='))
		env->value = ft_substr(value, (ft_strchr(value, '=') - value)
				+ 1, ft_strlen(value));
	env->next = NULL;
	return (env);
}

void	init_global_data(void)
{
	char	**env;
	char	*buf;
	char	*tmp;

	env = environ;
	while (env && *env)
	{
		if (ft_strncmp(*env, "OLDPWD", 6) && ft_strncmp(*env, "PWD", 3))
			envadd_back(&g_data.env, envnew(*env));
		env++;
	}
	envadd_back(&g_data.env, envnew("OLDPWD"));
	buf = getcwd(0, 0);
	tmp = ft_strjoin("PWD=", buf);
	free(buf);
	envadd_back(&g_data.env, envnew(tmp));
	free(tmp);
	g_data.exit_status = 0;
	g_data.ast.root = NULL;
	g_data.lexer.head = NULL;
	g_data.lexer.tail = NULL;
	g_data.lexer.size = 0;
}

void	reset_global_data(void)
{
	g_data.ast.root = NULL;
	g_data.lexer.head = NULL;
	g_data.lexer.tail = NULL;
	g_data.lexer.size = 0;
}
