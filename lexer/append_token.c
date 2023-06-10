/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:10:42 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/10 00:38:53 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	lexer_escape(t_linkedlist *list, char **line, int state)
// {
// 	if (state == GENERAL || (*line + 1  && *(*line + 1) == ESCAPE
// 			&& state == IN_DOUBLE_QUOTE))
// 		list_add_back(list, list_new_elem(*line + 1, 1, **line, state));
// 	else
// 		list_add_back(list, list_new_elem(*line, 2, **line, state));
// 	*line += 2 - !*(*line + 1);
// }

void	lexer_double_pipe(t_linkedlist *list, char **line, int state)
{
	list_add_back(list, list_new_elem(*line, 2,
			LOGICAL_OR_OPERATOR, state));
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
	{
		list_add_back(list, list_new_elem(*line, 1, **line, GENERAL));
		*state = **line;
	}
	else if (*state == **line)
	{
		list_add_back(list, list_new_elem(*line, 1, **line, GENERAL));
		*state = GENERAL;
	}
	else
		list_add_back(list, list_new_elem(*line, 1, **line, *state));
	(*line)++;
}
