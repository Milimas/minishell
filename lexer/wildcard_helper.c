/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:49:46 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/02 20:50:21 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*skip_currdir(char *path)
{
	while (!ft_strncmp(path, "./", 2))
		path += 2;
	return (path);
}

int	is_regular_file(const char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	return (S_ISREG(path_stat.st_mode));
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

int	is_hidden(char *d_name, char *pattern)
{
	if (!d_name || !pattern || (*pattern != '.'
			&& *d_name == '.' && *(d_name + 1) != '/'))
		return (0);
	return (1);
}

char	*join_path(char *path, char *d_name, char *sep)
{
	char	*new_path;

	if (path && *path)
	{
		if (ft_strlen(path) && path[ft_strlen(path) - 1] != '/')
			path = ft_strjoin(path, sep);
		else
			path = ft_strdup(path);
		new_path = ft_strjoin(path, d_name);
		free(path);
		return (new_path);
	}
	return (ft_strdup(d_name));
}
