/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_helper2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 06:16:52 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/03 06:17:14 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	match_f(char *d_name, char *pattern)
{
	d_name = skip_currdir(d_name);
	if (!*pattern && !*d_name)
		return (1);
	if (!ft_strncmp(pattern, "*", 1))
	{
		while (*(pattern + 1) == '*')
			pattern++;
		if (!*(pattern + 1))
			return (1);
		while (*d_name)
		{
			if (match_f(d_name, pattern + 1))
				return (1);
			d_name++;
		}
	}
	else if (*d_name && (*pattern == *d_name))
		return (match_f(d_name + 1, pattern + 1));
	return (0);
}
