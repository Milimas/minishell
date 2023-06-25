/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:58:41 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/25 22:09:40 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	lexer_expand_env(t_linkedlist *list)
{
	char	**split;
	char	*value;

	split = NULL;
	value = get_env_variable(list->tail->content + 1);
	if (list->tail->state == IN_DOUBLE_QUOTE)
	{
		list_add_back(list, list_new_elem(
				ft_strdup(value),
				ft_strlen(value), WORD, list->tail->state));
		delete_from_list(&list, list->tail->prev);
	}
	else
	{
		split = ft_split(value, ' ');
		list->tail->content = ft_strdup(*split);
		list->tail->len = ft_strlen(list->tail->content);
		list->tail->type = WORD;
		while (*split && *++split)
		{
			list_add_back(list, list_new_elem(" ", 1, WHITE_SPACE, list->tail->state));
			list_add_back(list, list_new_elem(ft_strdup(*split), ft_strlen(*split), WORD, list->tail->state));
			free(*split);
		}
	}
}

void	lexer_env(t_linkedlist *list, char **line, int state)
{
	if (is_token(*(*line + 1)) && *(*line + 1) != WHITE_SPACE && state != IN_QUOTE)
	{
		(*line)++;
		return ;
	}
	list_add_back(list, list_new_elem(*line, env_len((*line) + 1) + 1, ENV, state));
	*line += env_len((*line) + 1) + 1;
	if (list->tail && list->tail->state != IN_QUOTE && *(list->tail->content + 1))
		lexer_expand_env(list);
}

void	lexer_word(t_linkedlist *list, char **line, int state)
{
	list_add_back(list, list_new_elem(*line, word_len(*line), WORD, state));
	*line += word_len(*line);
}

t_linkedlist	*ft_lexer(char *line)
{
	int				state;

	state = GENERAL;
	while (line && *line)
	{
		if (ft_isspace(*line))
			*line = WHITE_SPACE;
		if (is_token(*line) && *line != ENV)
		{
			if (*line == PIPE_LINE && *(line + 1) == PIPE_LINE)
				lexer_double_pipe(&g_data.lexer, &line, state);
			else if (*line == '&' && *(line + 1) == '&')
				lexer_double_ampersand(&g_data.lexer, &line, state);
			else if (is_double_redirection(line) || *line == REDIRECTION_IN || *line == REDIRECTION_OUT)
				lexer_redirection(&g_data.lexer, &line, state);
			else if (*line == QUOTE || *line == DOUBLE_QUOTE)
				lexer_quotes(&g_data.lexer, &line, &state);
			else
			{
				list_add_back(&g_data.lexer, list_new_elem(line, 1, *line, state));
				line++;
			}
		}
		else if (line && *line == ENV)
			lexer_env(&g_data.lexer, &line, state);
		else
			lexer_word(&g_data.lexer, &line, state);
		if (g_data.lexer.tail && ft_strchr(g_data.lexer.tail->content, WILDCARD))
			g_data.lexer.tail->type = WILDCARD;
		if (g_data.lexer.tail && g_data.lexer.tail->type == WILDCARD)
			lexer_wildcard(&g_data.lexer, g_data.lexer.tail, state);
	}
	return (&g_data.lexer);
}
