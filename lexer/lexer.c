/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:58:41 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/23 21:46:42 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	word_len(char *line)
{
	char	*_line;

	_line = line;
	while (*line && ft_isalnum(*line))
		line++;
	return (line - _line);
}

void	lexer_expand_env(t_linkedlist *list)
{
	char	**split;

	if (get_env_variable(list->tail->content + 1))
	{
		if (list->tail->state == IN_DOUBLE_QUOTE)
		{
			list->tail->content = ft_strdup(
					get_env_variable(list->tail->content + 1));
			list->tail->len = ft_strlen(list->tail->content);
			list->tail->type = WORD;
		}
		else
		{
			split = ft_split(get_env_variable(list->tail->content + 1), ' ');
			list->tail->content = *split;
			list->tail->len = ft_strlen(*split);
			list->tail->type = WORD;
			while (*++split)
			{
				list_add_back(list, list_new_elem(" ", 1, WHITE_SPACE, list->tail->state));
				list_add_back(list, list_new_elem(*split, ft_strlen(*split), WORD, list->tail->state));
			}
		}
	}
}

void	lexer_env(t_linkedlist *list, char **line, int state)
{
	list_add_back(list, list_new_elem(*line, word_len((*line) + 1) + 1, ENV, state));
	*line += word_len((*line) + 1) + 1;
	if (list->tail->state != IN_QUOTE && *(list->tail->content + 1))
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
			else if (is_double_redirection(line))
				lexer_double_redirection(&g_data.lexer, &line, state);
			else if (*line == QUOTE || *line == DOUBLE_QUOTE)
				lexer_quotes(&g_data.lexer, &line, &state);
			else
			{
				list_add_back(&g_data.lexer, list_new_elem(line, 1, *line, state));
				line++;
			}
		}
		else if (*line == ENV)
			lexer_env(&g_data.lexer, &line, state);
		else
			lexer_word(&g_data.lexer, &line, state);
		if (ft_strchr(g_data.lexer.tail->content, WILDCARD))
			g_data.lexer.tail->type = WILDCARD;
		if (g_data.lexer.tail->type == WILDCARD)
			lexer_wildcard(&g_data.lexer, g_data.lexer.tail, state);
	}
	return (&g_data.lexer);
}
