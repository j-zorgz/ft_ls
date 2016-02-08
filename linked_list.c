/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcamhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/22 02:52:25 by jcamhi            #+#    #+#             */
/*   Updated: 2016/02/08 19:31:49 by jcamhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

static char	handle_type(mode_t mode)
{
	if ((mode & S_IFMT) == S_IFREG)
		return ('-');
	else if ((mode & S_IFMT) == S_IFIFO)
		return ('p');
	else if ((mode & S_IFMT) == S_IFCHR)
		return ('c');
	else if ((mode & S_IFMT) == S_IFDIR)
		return ('d');
	else if ((mode & S_IFMT) == S_IFBLK)
		return ('b');
	else if ((mode & S_IFMT) == S_IFLNK)
		return ('l');
	else if ((mode & S_IFMT) == S_IFSOCK)
		return ('s');
	else
		return ('-');
}

void	get_rights(t_file *ret, mode_t mode)
{
	ret->rights[0] = handle_type(mode);
	ret->rights[1] = (mode & S_IRUSR ? 'r' : '-');
	ret->rights[2] = (mode & S_IWUSR ? 'w' : '-');
	ret->rights[3] = (mode & S_IXUSR ? 'x' : '-');
	ret->rights[4] = (mode & S_IRGRP ? 'r' : '-');
	ret->rights[5] = (mode & S_IWGRP ? 'w' : '-');
	ret->rights[6] = (mode & S_IXGRP ? 'x' : '-');
	ret->rights[7] = (mode & S_IROTH ? 'r' : '-');
	ret->rights[8] = (mode & S_IWOTH ? 'w' : '-');
	ret->rights[9] = (mode & S_IXOTH ? 'x' : '-');
	ret->rights[10] = '\0';
}

t_file	*create_elem(const char *path, t_file *suivant, char *name)
{
	t_file *ret;
	t_stat	structure;
	char	*c_time;
	char	*join;
	ssize_t	path_size;
	char	lnk_path[PATH_MAX];

	ret = (t_file *)malloc(sizeof(t_file));
	join = ft_strjoin(path, name);
	if (lstat(join, &structure) == -1)
	{
		ft_printf("Name : %s Path: %s Join: %s\n", name, path, join);
		print_error((char*)path);
		return (0);
	}
	if ((structure.st_mode & S_IFMT) == S_IFLNK)
	{
		path_size = readlink(join, lnk_path, PATH_MAX - 1);
		lnk_path[path_size] = '\0';
		ret->path_to_lnk = ft_strdup(lnk_path);
	}
	else
		ret->path_to_lnk = NULL;
	free(join);
	ft_strncpy(ret->name, name, PATH_MAX);
	ret->name_len = ft_strlen(ret->name);
	ret->path = ft_strdup(path);
	get_rights(ret, structure.st_mode);
	ret->nlinks = structure.st_nlink;
	ret = set_own_grp(structure, ret);
	ret->nbytes = structure.st_size;
	ret->m_time = structure.st_mtimespec;
	c_time = ctime(&((ret->m_time).tv_sec));
	ret->day_name = ft_strsub(c_time, 0, 3);
	ret->mounth = ft_strsub(c_time, 4, 3);
	ret->day_nbr = ft_strsub(c_time, 8, 2);
	ret->hour = ft_strsub(c_time, 11, 2);
	ret->min = ft_strsub(c_time, 14, 2);
	ret->sec = ft_strsub(c_time, 17, 2);
	ret->blocks = structure.st_blocks;
	ret->next = suivant;
	return (ret);
}

void	add_elem_end(const char *path, t_file *list, char *name)
{
	t_file *elem;

	if (!(elem = create_elem(path, NULL, name)))
		return ;
	while (list->next != NULL)
		list = list->next;
	list->next = elem;
}

void	add_elem_start(const char *path, t_file **list, char *name)
{
	t_file	*elem;

	elem = create_elem(path, *list, name);
	*list = elem;
}
