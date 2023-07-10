/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 01:56:18 by abeihaqi          #+#    #+#             */
/*   Updated: 2022/10/16 03:34:09 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_digits(int n)
{
	size_t	size;

	size = 0;
	while (n)
	{
		n /= 10;
		size++;
	}
	return (size);
}

static char	*conv(int n, int is_signed)
{
	char	*s;
	size_t	size;

	size = ft_digits(n) + is_signed + 1;
	s = (char *)ft_calloc(size, sizeof(char));
	if (!s)
		return (NULL);
	if (is_signed)
		*s = '-';
	while (n)
	{
		s[--size - 1] = n % 10 + '0';
		n /= 10;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	int		is_signed;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	is_signed = 0;
	if (n < 0)
	{
		n = -n;
		is_signed = 1;
	}
	return (conv(n, is_signed));
}
