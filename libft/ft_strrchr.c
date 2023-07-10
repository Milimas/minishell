/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 02:48:29 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/18 08:43:07 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;

	if ((unsigned char)c > 127)
		return ((char *)s);
	str = (char *)s;
	while (*str)
		str++;
	while ((unsigned char)*str != (unsigned char)c && str != s)
		str--;
	if ((unsigned char)*str != (unsigned char)c)
		return (NULL);
	return (str);
}
