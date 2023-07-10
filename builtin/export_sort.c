/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 03:15:22 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/04 15:57:56 by rouarrak         ###   ########.fr       */
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

void	sort_chartab(char **tab, int size)
{
	int		i;
	char	*swap;

	i = 0;
	while (i + 1 < size)
	{
		if (ft_strcmp(tab[i], tab[i + 1]) >= 0)
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

static void	print_export_prime(int fd, t_env *env)
{
	if (!ft_strcmp(env->key, "_"))
		return ;
	ft_putstr_fd("declare -x ", fd);
	ft_putstr_fd(env->key, fd);
	if (env->value)
	{
		ft_putstr_fd("=\"", fd);
		ft_putstr_fd(env->value, fd);
		ft_putstr_fd("\"", fd);
	}
	ft_putchar_fd('\n', fd);
}

void	print_export(char **tab, int size, int fd)
{
	t_env	*tmp;
	t_env	*env;
	int		i;

	env = g_data.env;
	tmp = g_data.env;
	i = 0;
	while (env && i < size)
	{
		if (ft_strcmp(env->key, tab[i]) == 0)
		{	
			print_export_prime(fd, env);
			env = tmp;
			i++;
		}
		else
			env = env->next;
	}
}

void	exp_tab(t_env *env, int fd)
{
	char	**tab;
	int		size;
	int		i;

	i = 0;
	size = ft_envsize(env);
	tab = malloc(size * sizeof(char *));
	while (env)
	{
		tab[i] = env->key;
		env = env->next;
		i++;
	}
	sort_chartab(tab, size);
	print_export(tab, size, fd);
	free(tab);
}
