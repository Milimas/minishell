/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:07:16 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/12 02:10:19 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_token(char c)
{
	return (ft_isspace(c) || c == NEW_LINE || c == QUOTE
		|| c == DOUBLE_QUOTE || c == ENV || c == '&'
		|| c == PIPE_LINE || c == REDIRECTION_IN
		|| c == REDIRECTION_OUT
		|| c == PARENTASIS_OPEN || c == PARENTASIS_CLOSE);
}
