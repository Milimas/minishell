/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 00:13:11 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/18 05:59:42 by abeihaqi         ###   ########.fr       */
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

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;

	if (!s)
		return (NULL);
	if (start >= (unsigned int)ft_strlen(s))
		return (ft_calloc(1, sizeof(char)));
	len = ft_strnlen(s + start, len);
	sub = (char *)malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	ft_memcpy(sub, s + start, len);
	sub[len] = 0;
	return (sub);
}
