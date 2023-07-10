/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 02:46:39 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/18 08:37:22 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <ctype.h>

char	*ft_strchr(const char *s, int c)
{
	if ((unsigned char)c > 127)
		return ((char *)s);
	while (*s && (unsigned char)*s != (unsigned char)c)
		s++;
	if ((unsigned char)*s != (unsigned char)c)
		return (NULL);
	return ((char *)s);
}
