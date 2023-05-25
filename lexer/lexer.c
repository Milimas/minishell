/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:58:41 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 10:03:05 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_linkedlist	*ft_lexer(char *line)
{
	t_linkedlist	*list;
	t_elem			elem;
	int				state;

	list = list_init(NULL);
	state = GENERAL;
	while (line && *line)
	{
		if (ft_isspace(*line))
			*line = WHITE_SPACE;
		if (is_token(*line) && *line != ENV)
		{
			if (is_double_redirection(line))
				lexer_double_redirection(list, &line, state);
			else if (*line == ESCAPE)
				lexer_escape(list, &line, state);
			else if (*line == QUOTE || *line == DOUBLE_QUOTE)
				lexer_quotes(list, &line, &state);
			else
			{
				list_add_back(list, list_new_elem(line, 1, *line, state));
				line++;
			}
		}
		else
		{
			elem.content = line;
			elem.len = 0;
			elem.type = WORD;
			if (*line == ENV)
			{
				elem.type = ENV;
			}
			elem.state = state;
			while (*line)
			{
				line++;
				elem.len++;
				if (is_token(*line) && *line != ENV)
					break ;
			}
			list_add_back(list, list_new_elem(elem.content, elem.len, elem.type, elem.state));
			if (list->head->type == ENV)
			{
				list->head->content = ft_strdup(get_env_variable(list->head->content + 1));
			}
		}
	}
	return (list);
}
