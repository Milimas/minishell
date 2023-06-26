/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 07:21:53 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/26 07:22:27 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*out_quotes(int len, char *str, int nb, char *res)
{
	int		i;
	int		j;

	len = len - nb;
	res = malloc(len + 1 * sizeof(char));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (j < len && str[i] != '\"')
		{
			res[j] = str[i];
			j++;
		}
		i++;
	}
	res[j] = '\0';
	return (res);
}

char	*rm_quotes(char *str)
{
	int		nb;
	int		i;
	int		len;
	char	*res;

	nb = 0;
	i = 0;
	res = NULL;
	len = ft_strlen(str);
	while (str[i])
	{
		if (str[i] == '\"')
			nb++;
		i++;
	}
	if (nb % 2 == 0)
	{
		res = out_quotes(len, str, nb, res);
	}
	return (res);
}

void	expanding(char *str, int fd, char *limiter)
{
	char	*tmp;
	char	*value;

	if (!str || !ft_strcmp(str, limiter))
		return ;
	while (*str)
	{
		if (*str == '$' && env_len(str + 1))
		{
			str++;
			tmp = ft_substr(str, 0, env_len(str));
			value = get_env_variable(tmp);
			ft_putstr_fd(value, fd);
			str += env_len(str) - 1;
			free(tmp);
			free(value);
		}
		else
			ft_putchar_fd(*str, fd);
		if (*str)
			str++;
	}
}