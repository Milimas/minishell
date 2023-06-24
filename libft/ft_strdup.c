/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 23:29:17 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/24 04:17:39 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	if (!s1 || !*s1)
		return (ft_calloc(sizeof(char), 1));
	return (ft_substr(s1, 0, ft_strlen(s1)));
}
