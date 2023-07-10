/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 23:14:28 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/16 01:51:00 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const char	*s;
	char		*d;

	s = (const char *)src;
	d = (char *)dst;
	if (!s && !d)
		return (NULL);
	while (n--)
		*d++ = *s++;
	return (dst);
}
