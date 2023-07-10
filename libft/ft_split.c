/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 04:49:30 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/17 03:36:24 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wordcount(char const *s, char c)
{
	int	wc;

	wc = 0;
	while (*s)
	{
		while (*s && !ft_memcmp(s, &c, 1))
			s++;
		if (*s)
			wc++;
		while (*s && ft_memcmp(s, &c, 1))
			s++;
	}
	return (wc);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	char	**a;
	size_t	size;

	if (!s)
		return (NULL);
	size = ft_wordcount(s, c) + 1;
	arr = (char **)malloc(size * sizeof(char *));
	if (!arr)
		return (NULL);
	a = arr;
	while (*s)
	{
		while (*s && !ft_memcmp(s, &c, 1))
			s++;
		if (*s && ft_strchr(s, c))
			*a++ = ft_substr(s, 0, ft_strchr(s, c) - s);
		else if (*s)
			*a++ = ft_substr(s, 0, ft_strchr(s, 0) - s);
		while (*s && ft_memcmp(s, &c, 1))
			s++;
	}
	*a = 0;
	return (arr);
}
