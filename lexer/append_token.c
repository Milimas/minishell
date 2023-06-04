/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:10:42 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/04 16:18:58 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	lexer_escape(t_linkedlist *list, char **line, int state)
{
	if (state == GENERAL || (*(*line + 1) == ESCAPE && state == IN_DOUBLE_QUOTE))
		list_add_back(list, list_new_elem(*line + 1, 1, **line, state));
	else
		list_add_back(list, list_new_elem(*line, 2, **line, state));
	*line += 2;
}

void	lexer_double_redirection(t_linkedlist *list, char **line, int state)
{
	list_add_back(list, list_new_elem(*line, 2,
			is_double_redirection(*line), state));
	*line += 2;
}

void	lexer_quotes(t_linkedlist *list, char **line, int *state)
{
	if (*state == GENERAL)
		*state = **line;
	else if (*state == **line)
		*state = GENERAL;
	else
		list_add_back(list, list_new_elem(*line, 1, **line, *state));
	(*line)++;
}
