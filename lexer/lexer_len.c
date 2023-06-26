/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_len.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 22:09:43 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/26 04:58:52 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	filename_len(char *line)
{
	char	*_line;

	_line = line;
	while (*line && *line != ' ')
		line++;
	return (line - _line);
}

int	word_len(char *line)
{
	char	*_line;

	_line = line;
	while (*line && !is_token(*line))
		line++;
	return (line - _line);
}

int	env_len(char *line)
{
	char	*_line;

	_line = line;
	if (*line == '?')
		return (1);
	while (*line && (ft_isalnum(*line) || *line == '_'))
		line++;
	return (line - _line);
}
