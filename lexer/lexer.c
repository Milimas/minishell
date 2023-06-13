/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:58:41 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/12 03:44:55 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	word_len(char *line)
{
	char	*_line;

	_line = line;
	while (*line && !is_token(*line))
		line++;
	return (line - _line);
}

void	lexer_expand_env(t_elem **elem)
{
	if (get_env_variable((*elem)->content + 1))
	{
		(*elem)->content = ft_strdup(
				get_env_variable((*elem)->content + 1));
		(*elem)->len = ft_strlen((*elem)->content);
		(*elem)->type = WORD;
	}
}

void	lexer_env(t_linkedlist *list, char **line, int state)
{
	list_add_back(list, list_new_elem(*line, word_len((*line) + 1) + 1, ENV, state));
	*line += word_len((*line) + 1) + 1;
	if (list->tail->state != IN_QUOTE && *(list->tail->content + 1))
		lexer_expand_env(&list->tail);
}

void	lexer_word(t_linkedlist *list, char **line, int state)
{
	list_add_back(list, list_new_elem(*line, word_len(*line), WORD, state));
	*line += word_len(*line);
}

t_linkedlist	*ft_lexer(char *line)
{
	t_linkedlist	*list;
	int				state;

	list = list_init(NULL);
	state = GENERAL;
	while (line && *line)
	{
		if (ft_isspace(*line))
			*line = WHITE_SPACE;
		if (is_token(*line) && *line != ENV)
		{
			if (*line == PIPE_LINE && *(line + 1) == PIPE_LINE)
				lexer_double_pipe(list, &line, state);
			else if (*line == '&' && *(line + 1) == '&')
				lexer_double_ampersand(list, &line, state);
			else if (is_double_redirection(line))
				lexer_double_redirection(list, &line, state);
			else if (*line == QUOTE || *line == DOUBLE_QUOTE)
				lexer_quotes(list, &line, &state);
			else
			{
				list_add_back(list, list_new_elem(line, 1, *line, state));
				line++;
			}
		}
		else if (*line == ENV)
			lexer_env(list, &line, state);
		else
			lexer_word(list, &line, state);
		if (ft_strchr(list->tail->content, WILDCARD))
			list->tail->type = WILDCARD;
		if (list->tail->type == WILDCARD)
			lexer_wildcard(list, list->tail, state);
	}
	return (list);
}
