/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 07:21:53 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/04 11:48:23 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*out_quotes(int len, char *str, int nb, char *res)
{
	int		i;
	int		j;
	char	c;

	len = len - nb;
	c = which_quote(str);
	res = malloc(len + 1 * sizeof(char));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (j < len && str[i] != c)
		{
			res[j] = str[i];
			j++;
		}
		i++;
	}
	res[j] = '\0';
	return (res);
}

char	which_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			return ('\"');
		if (str[i] == '\'')
			return ('\'');
		i++;
	}
	return (0);
}

int	check_limiter(char *str)
{
	int		i;
	int		nb;
	char	q;

	i = 0;
	nb = 0;
	q = which_quote(str);
	if (!q)
		return (1);
	while (str[i])
	{
		if (str[i] == q)
			nb++;
		i++;
	}
	if (nb == 2)
		return (1);
	return (0);
}

char	*rm_quotes(char *str)
{
	int		nb;
	int		i;
	int		len;
	char	*res;
	char	c;

	nb = 0;
	i = 0;
	c = which_quote(str);
	res = NULL;
	len = ft_strlen(str);
	while (str[i])
	{
		if (str[i] == c)
			nb++;
		i++;
	}
	if (nb % 2 == 0)
		res = out_quotes(len, str, nb, res);
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
