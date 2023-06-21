/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 03:15:22 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/21 04:02:07 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_envsize(t_env *lst)
{
	int		size;
	t_env	*current;

	size = 0;
	current = lst;
	while (current != NULL)
	{
		current = current->next;
		size++;
	}
	return (size);
}

void    sort_chartab(char **tab, int size)
{
    int i;
    char *swap;
	i = 0;
	while (i + 1 <size)
	{
		if(ft_strcmp(tab[i], tab[i + 1]) >= 0)
		{
			swap = tab[i];
			tab[i] = tab[i + 1];
			tab[i + 1] = swap;
			i = 0;
		}
		else
			i++;
	}
}

void	print_export(char **tab, int size)
{
	t_env	*tmp;
	t_env	*env;
	int		i;

	env = g_data.env;
	tmp = g_data.env;
	i = 0;
	while (env && i < size)
	{
		if(ft_strcmp(env->key, tab[i]) == 0)
		{	
			printf("declare -x %s", env->key);
			if (env->value)
				printf("=\"%s\"", env->value);
			printf("\n");
			env = tmp;
			i++;
		}
		else
			env = env->next;
	}
}
void	exp_tab(t_env *env)
{
	char	**tab;
	int		size;
	int		i;

	i = 0;
	size = ft_envsize(env);
	tab = malloc(size * sizeof(char*));
	while (env)
	{
		tab[i] = env->key;
		env = env->next;
		i++;
	}
	
    sort_chartab(tab,size);
	print_export(tab, size);
	
	free(tab);
}
