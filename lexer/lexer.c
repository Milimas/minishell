/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 06:58:41 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/27 18:32:35 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	lexer_word(t_linkedlist *list, char **line, int state)
{
	list_add_back(list, list_new_elem(*line, word_len(*line), WORD, state));
	*line += word_len(*line);
}

void	ft_token_lexer(char **line, int *state)
{
	if (*(*line) == PIPE_LINE && *((*line) + 1) == PIPE_LINE)
		lexer_double_pipe(&g_data.lexer, line, *state);
	else if (**line == '&' && *((*line) + 1) == '&')
		lexer_double_ampersand(&g_data.lexer, line, *state);
	else if ((is_double_redirection(*line)
		|| **line == REDIRECTION_IN || **line == REDIRECTION_OUT)
		&& *state == GENERAL)
		lexer_redirection(&g_data.lexer, line, *state);
	else if (**line == QUOTE || **line == DOUBLE_QUOTE)
		lexer_quotes(&g_data.lexer, line, state);
	else
	{
		list_add_back(&g_data.lexer, list_new_elem(*line, 1, **line, *state));
		(*line)++;
	}
}

int	is_after_redir()
{
	t_elem	*elem;

	if (!g_data.lexer.tail)
		return (EXIT_FAILURE);
	elem = g_data.lexer.tail->prev;
	while (elem)
	{
		if (elem->type != WHITE_SPACE)
			break ;
		elem = elem->prev;
	}
	if (elem && is_redirection(elem))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
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
			ft_token_lexer(&line, &state);
		else if (line && *line == ENV && is_after_redir())
			lexer_env(&g_data.lexer, &line, state);
		else
			lexer_word(&g_data.lexer, &line, state);
		if (g_data.lexer.tail
			&& ft_strchr(g_data.lexer.tail->content, WILDCARD))
			g_data.lexer.tail->type = WILDCARD;
		if (g_data.lexer.tail && g_data.lexer.tail->type == WILDCARD
			&& is_after_redir())
			lexer_wildcard(&g_data.lexer, g_data.lexer.tail, state);
	}
	return (&g_data.lexer);
}
