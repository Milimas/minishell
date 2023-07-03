/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 22:07:28 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/03 06:17:11 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_files(t_list *files_tmp, t_linkedlist *list)
{
	t_list	*tmp;

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
}

void	lexer_wildcard(t_linkedlist *list, t_elem *elem, int state)
{
	t_list	*files;
	t_list	*files_tmp;

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
	add_files(files_tmp, list);
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
