/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 16:22:53 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/18 10:12:54 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	if (!n)
		return (NULL);
	while (--n && *str != (unsigned char)c)
		str++;
	if (n == 0 && *str != (unsigned char)c)
		return (NULL);
	return ((void *)str);
}
