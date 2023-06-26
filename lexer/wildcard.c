/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 22:07:28 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/26 14:15:54 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	lexer_wildcard(t_linkedlist *list, t_elem *elem, int state)
{
	t_list	*files;
	t_list	*files_tmp;

	(void)list;
	files = NULL;
	if (state != GENERAL || !elem)
		return ;
	if (ft_strrchr(elem->content, '/') && *(ft_strrchr(elem->content, '/') + 1) == 0)
		files = get_files("", ft_split(elem->content, '/'), DT_DIR);
	else if (*elem->content == '.')
		files = get_files("", ft_split(ft_strchr(elem->content, '/'), '/'), DT_REG);
	else
		files = get_files("", ft_split(elem->content, '/'), DT_REG);
	if (!files)
		return ;
	sort_list(files);
	files_tmp = files->next;
	if (files)
	{
		list->tail->content = ft_strdup(files->content);
		list->tail->len = ft_strlen(files->content);
		list->tail->type = WORD;
	}
	while (files_tmp)
	{
		list_add_back(list, list_new_elem(" ", 1, WHITE_SPACE, GENERAL));
		list_add_back(list, list_new_elem(files_tmp->content, ft_strlen(files_tmp->content), WORD, GENERAL));
		files_tmp = files_tmp->next;
	}
}

void	sort_list(t_list *list)
{
	t_list	*tmp;
	char	*ctmp;

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

// t_list	*get_files(char *path, char **pattern, unsigned char d_type)
// {
// 	DIR				*dirp;
// 	struct dirent	*dir;
// 	char			*npath;
// 	t_list			*list;

// 	list = NULL;
// 	dirp = opendir(path);
// 	if (!dirp)
// 		return (NULL);
// 	dir = readdir(dirp);
// 	while (dir)
// 	{
// 		if (**pattern != '.' && (!ft_strncmp(dir->d_name, ".", 2) || !ft_strncmp(dir->d_name, "..", 3)))
// 		{
// 			dir = readdir(dirp);
// 			continue ;
// 		}
// 		if (*pattern && match_f(dir->d_name, *pattern))
// 		{
// 			if (dir->d_type == DT_DIR && *(pattern + 1))
// 			{
// 				npath = ft_strjoin(path, "/");
// 				npath = ft_strjoin(npath, dir->d_name);
// 				return (get_files(npath, pattern + 1, d_type));
// 			}
// 			if (((d_type == DT_DIR && dir->d_type == d_type) || d_type == DT_REG) && !*(pattern + 1))
// 				ft_lstadd_back(&list, ft_lstnew(dir->d_name));
// 		}
// 		dir = readdir(dirp);
// 	}
// 	return (list);
// }

t_list	*get_files(char *path, char **pattern, unsigned char d_type)
{
	DIR				*dirp;
	struct dirent	*dir;
	char			*npath;
	t_list			*list;

	list = NULL;
	if (path && *path == 0)
		dirp = opendir(".");
	else
		dirp = opendir(path);
	if (!dirp)
		return (NULL);
	dir = readdir(dirp);
	while (dir)
	{
		// if (**pattern != '.' && (!ft_strncmp(dir->d_name, ".", 2) || !ft_strncmp(dir->d_name, "..", 3)))
		// {
		// 	dir = readdir(dirp);
		// 	continue ;
		// }
		if (*pattern && match_f(dir->d_name, *pattern))
		{
			if (dir->d_type == DT_DIR && *(pattern + 1))
			{
				if (*path)
					npath = ft_strjoin(path, "/");
				else
					npath = ft_strdup("./");
				npath = ft_strjoin(npath, dir->d_name);
				ft_lstadd_back(&list, get_files(npath, pattern + 1, d_type));
			}
			if (((d_type == DT_DIR && dir->d_type == d_type) || d_type == DT_REG) && !*(pattern + 1))
			{
				if  (*path)
					npath = ft_strjoin(path, "/");
				else
					npath = path;
				npath = ft_strjoin(npath, dir->d_name);
				if ((d_type == DT_DIR && dir->d_type == d_type))
					npath = ft_strjoin(npath, "/");
				ft_lstadd_back(&list, ft_lstnew(npath));
			}
		}
		dir = readdir(dirp);
	}
	return (list);
}
