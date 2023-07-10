/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_plus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:01:34 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/01 23:30:28 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	plus_check(char *args)
{
	int	i;
	int	cpt;

	i = 0;
	cpt = 0;
	while (args[i] && args[i] != '=')
		i++;
	i--;
	while (args[i] && args[i] == '+')
	{
		i--;
		cpt++;
	}
	return (cpt);
}
