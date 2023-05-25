/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:54:21 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 06:56:14 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bsh_echo(t_elem *lexer)
{
	int		n;
	char	*flag;

	n = 1;
	// if (lexer->next)
		lexer = lexer->next;
	while ((lexer && ft_strnstr(lexer->content, "-", ft_strlen(lexer->content) + 1)) || (lexer && *lexer->content == ' '))
	{
		if (*lexer->content == ' ')
		{
			lexer = lexer->next;
			continue ;
		}
		flag = lexer->content;
		while (++flag && *flag)
		{
			if (!ft_strchr("neE", *flag))
			{
				flag = NULL;
				break ;
			}
		}
		if (!flag)
			break ;
		lexer = lexer->next;
		n = 0;
	}
	while (lexer)
	{
		ft_putstr_fd(lexer->content, 1);
		lexer = lexer->next;
		// write(1, " ", !!(lexer));
	}
	write(1, "\n", 1 * n);
}
