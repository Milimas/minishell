/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 17:12:09 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/18 11:40:13 by abeihaqi         ###   ########.fr       */
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

char	*ft_strnstr(const char *hystack, const char *needle, size_t len)
{
	const char	*hystack_end;

	if (!hystack && !len)
		return (NULL);
	if (!*hystack && !*needle)
		return ((char *)hystack);
	if (ft_strnlen(hystack, len) < ft_strlen(needle)
		|| (!len && *needle))
		return (NULL);
	hystack_end = hystack + ft_strnlen(hystack, len) - ft_strnlen(needle, len);
	while (hystack <= hystack_end)
	{
		if (!ft_strncmp(hystack, needle, ft_strnlen(needle, len)))
			return ((char *)hystack);
		hystack++;
	}
	return (NULL);
}
