/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 01:04:28 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/18 11:28:23 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_strnlen(const char *s, size_t maxlen)
{
	const char	*str;

	str = s;
	while (*str && (size_t)(str - s) < maxlen)
		str++;
	return (str - s);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dlen;
	size_t	slen;

	slen = ft_strlen(src);
	if (!dstsize)
		return (slen);
	dlen = ft_strnlen(dst, dstsize);
	if (dstsize == dlen)
		return (dlen + slen);
	if (slen < dstsize - dlen)
		ft_memcpy(dst + dlen, src, slen + 1);
	else
	{
		ft_memcpy(dst + dlen, src, dstsize - dlen - 1);
		dst[dstsize - 1] = 0;
	}
	return (dlen + slen);
}
