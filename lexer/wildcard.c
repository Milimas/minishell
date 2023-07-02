/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 22:07:28 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/02 19:43:33 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	match_f(char *d_name, char *pattern)
{
	d_name = skip_currdir(d_name);
	if (!*pattern && !*d_name)
		return (1);
	if (!ft_strncmp(pattern, "*", 1))
	{
		while (*(pattern + 1) == '*')
			pattern++;
		if (!*(pattern + 1))
			return (1);
		while (*d_name)
		{
			if (match_f(d_name, pattern + 1))
				return (1);
			d_name++;
		}
	}
	else if (*d_name && (*pattern == *d_name))
		return (match_f(d_name + 1, pattern + 1));
	return (0);
}

void	lexer_wildcard(t_linkedlist *list, t_elem *elem, int state)
{
	t_list	*files;
	t_list	*files_tmp;
	t_list	*tmp;

	(void)list;
	files = NULL;
	if (state != GENERAL || !elem)
		return ;
	files = get_files(elem->content);
	if (!files)
		return ;
	sort_list(files);
	files_tmp = files->next;
	if (files)
	{
		free(list->tail->content);
		list->tail->content = ft_strdup(files->content);
		list->tail->len = ft_strlen(files->content);
		list->tail->type = WORD;
		free(files->content);
	}
	while (files_tmp)
	{
		list_add_back(list, list_new_elem(" ", 1, WHITE_SPACE, GENERAL));
		list_add_back(list, list_new_elem(files_tmp->content,
				ft_strlen(files_tmp->content), WORD, GENERAL));
		tmp = files_tmp;
		files_tmp = files_tmp->next;
		free(tmp->content);
		free(tmp);
	}
	free(files);
}

void	get_files_rec_helper(char *path, char *pattern,
	t_list **list, struct dirent *dir)
{
	char			*npath;

	npath = join_path(path, dir->d_name, "/");
	if (match_f(npath, skip_currdir(pattern)))
		ft_lstadd_back(list, ft_lstnew(npath));
	else if (dir->d_type == DT_DIR)
	{
		npath = ft_strconcat(npath, "/");
		if (ft_strrchr(pattern, '/')
			&& *(ft_strrchr(pattern, '/') + 1) == 0
			&& match_f(npath, skip_currdir(pattern)))
			ft_lstadd_back(list, ft_lstnew(npath));
		else if (ft_strcmp(dir->d_name, ".")
			&& ft_strcmp(dir->d_name, ".."))
		{
			free(npath);
			npath = ft_strjoin(path, dir->d_name);
			get_files_rec(npath, pattern, list);
			free(npath);
		}
		else
			free(npath);
	}
	else
		free(npath);
}

void	get_files_rec(char *path, char *pattern, t_list **list)
{
	DIR				*dirp;
	struct dirent	*dir;
	char			*tmp;

	tmp = NULL;
	if (!ft_strncmp(pattern, "./", 2) && ft_strncmp(path, "./", 2))
	{
		path = ft_strjoin("./", path);
		tmp = path;
	}
	if (*path)
		dirp = opendir(path);
	else
		dirp = opendir(".");
	if (!dirp)
		return ;
	dir = readdir(dirp);
	while (dir)
	{
		if (is_hidden(dir->d_name, skip_currdir(pattern)))
			get_files_rec_helper(path, pattern, list, dir);
		dir = readdir(dirp);
	}
	free(tmp);
	closedir(dirp);
}

t_list	*get_files(char *pattern)
{
	t_list	*list;
	char	*base_path;

	list = NULL;
	base_path = ft_strdup("");
	get_files_rec(base_path, pattern, &list);
	free(base_path);
	return (list);
}
