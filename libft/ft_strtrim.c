/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 00:41:23 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/18 11:00:00 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*end;
	char	*trimmed;
	size_t	maxlen;

	if (!s1)
		return (NULL);
	if (!*s1)
		return (ft_strdup(s1));
	end = ft_strchr(s1, 0) - 1;
	maxlen = 0;
	while (ft_strchr(set, *s1))
		s1++;
	while (ft_strchr(set, *end) && end >= s1)
		end--;
	if (end >= s1)
		maxlen = end - s1 + 1;
	trimmed = (char *)malloc((maxlen + 1) * sizeof(char));
	if (!trimmed)
		return (NULL);
	ft_memcpy(trimmed, s1, maxlen);
	trimmed[maxlen] = 0;
	return (trimmed);
}
