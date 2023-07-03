/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:10:42 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/03 14:02:32 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	lexer_double_pipe(t_linkedlist *list, char **line, int state)
{
	list_add_back(list, list_new_elem(*line, 2,
			LOGICAL_OR_OPERATOR, state));
	*line += 2;
}

void	lexer_double_ampersand(t_linkedlist *list, char **line, int state)
{
	list_add_back(list, list_new_elem(*line, 2,
			DOUBLE_AMPERSAND, state));
	*line += 2;
}

void	lexer_redirection(t_linkedlist *list, char **line, int state)
{
	if (is_double_redirection(*line))
	{
		list_add_back(list, list_new_elem(*line, 2,
				is_double_redirection(*line), state));
		*line += 2;
		if (list->tail->type == HERE_DOC)
		{
			while (**line == WHITE_SPACE)
				(*line)++;
			list_add_back(list, list_new_elem(*line,
					filename_len(*line), WORD, state));
			*line += filename_len(*line);
		}
	}
	else
	{
		list_add_back(list, list_new_elem(*line, 1,
				**line, state));
		(*line)++;
	}
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
