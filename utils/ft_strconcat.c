/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strconcat.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:15:08 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/26 03:24:39 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strconcat(char *s1, char *s2)
{
	char	*concat;

	if (!s1)
		s1 = ft_strdup("");
	concat = ft_strjoin(s1, s2);
	free(s1);
	return (concat);
}
