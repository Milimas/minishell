/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:10:42 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/24 18:36:21 by abeihaqi         ###   ########.fr       */
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
	}
	else
	{
		list_add_back(list, list_new_elem(*line, 1,
				**line, state));
		(*line)++;
	}
}

int	match_f(char *d_name, char *pattern)
{
	char	**patterns;
	int		check_start;
	int		check_end;

	if (!*pattern)
		return (0);
	pattern = ft_substr(pattern, 0, ft_strchr(pattern, '/') - pattern);
	check_start = (*pattern != '*');
	check_end = (pattern[ft_strlen(pattern) - 1] != '*');
	patterns = ft_split(pattern, '*');
	if (*d_name == '.' && *pattern != '.')
		return (0);
	if (!*patterns)
		return (1);
	if (check_start)
	{
		if (ft_strnstr(d_name, *patterns, ft_strlen(*patterns)) == d_name)
			patterns++;
		else
			return (0);
	}
	while (*patterns && *(patterns + check_end))
	{
		d_name = ft_strnstr(d_name, *patterns, ft_strlen(d_name)) + ft_strlen(*patterns) - 1;
		if (!d_name)
			return (0);
		patterns++;
	}
	if (*patterns && check_end)
	{
		if (ft_strnstr(d_name, *patterns, ft_strlen(d_name)) != d_name + ft_strlen(d_name) - ft_strlen(*patterns))
			return (0);
	}
	return (1);
}

int	is_regular_file(const char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	return (S_ISREG(path_stat.st_mode));
}

t_list	*get_files(char *path, char **pattern, unsigned char d_type)
{
	DIR				*dirp;
	struct dirent	*dir;
	char			*npath;
	t_list			*list;

	list = NULL;
	dirp = opendir(path);
	if (!dirp)
		return (NULL);
	dir = readdir(dirp);
	while (dir)
	{
		if (**pattern != '.' && (!ft_strncmp(dir->d_name, ".", 2) || !ft_strncmp(dir->d_name, "..", 3)))
		{
			dir = readdir(dirp);
			continue ;
		}
		if (*pattern && match_f(dir->d_name, *pattern))
		{
			if (dir->d_type == DT_DIR && *(pattern + 1))
			{
				npath = ft_strjoin(path, "/");
				npath = ft_strjoin(npath, dir->d_name);
				return (get_files(npath, pattern + 1, d_type));
			}
			if (((d_type == DT_DIR && dir->d_type == d_type) || d_type == DT_REG) && !*(pattern + 1))
				ft_lstadd_back(&list, ft_lstnew(dir->d_name));
		}
		dir = readdir(dirp);
	}
	return (list);
}

void	sort_list(t_list *list)
{
	t_list	*tmp;
	char	*ctmp;

	printf("sorting\n");
	while (list)
	{
		tmp = list;
		while (tmp)
		{
			if (ft_strcmp(list->content, tmp->content) > 0)
			{
				ctmp = list->content;
				list->content = tmp->content;
				tmp->content = ctmp;
			}
			tmp = tmp->next;
		}
		list = list->next;
	}
}

void	lexer_wildcard(t_linkedlist *list, t_elem *elem, int state)
{
	t_list	*files;
	t_list	*files_tmp;
	
	(void)list;
	files = NULL;
	if (state != GENERAL || !elem)
		return ;
	if (ft_strrchr(elem->content, '/') && *(ft_strrchr(elem->content, '/') + 1) == 0)
		files = get_files(".", ft_split(elem->content, '/'), DT_DIR);
	else
		files = get_files(".", ft_split(elem->content, '/'), DT_REG);
	sort_list(files);
	files_tmp = files->next;
	if (files)
	{
		list->tail->content = files->content;
		list->tail->len = ft_strlen(files->content);
		list->tail->type = WORD;
	}
	while (files_tmp)
	{
		list_add_back(list, list_new_elem(ft_strdup(" "), 1, WHITE_SPACE, GENERAL));
		list_add_back(list, list_new_elem(files_tmp->content, ft_strlen(files_tmp->content), WORD, GENERAL));
		files_tmp = files_tmp->next;
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
