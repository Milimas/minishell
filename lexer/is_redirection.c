/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:09:03 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 07:09:32 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_double_redirection(char *line)
{
	if ((*line == REDIRECTION_OUT && *(line + 1) == REDIRECTION_OUT))
		return (DOUBLE_REDIRECTION_OUT);
	if ((*line == REDIRECTION_IN && *(line + 1) == REDIRECTION_IN))
		return (HERE_DOC);
	return (0);
}
