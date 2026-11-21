/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfaris <cfaris@student.1337.ma>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-11-21 16:15:32 by cfaris            #+#    #+#             */
/*   Updated: 2026-11-21 16:15:32 by cfaris           ###   ########.1337     */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*free_stash(char **stash)
{
	free(*stash);
	*stash = NULL;
	return (NULL);
}

static char	*read_stash(int fd, char *stash, char *buf)
{
	ssize_t	ret;

	ret = 1;
	while (!ft_strchr_gnl(stash) && ret > 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret < 0)
		{
			free(stash);
			return (NULL);
		}
		buf[ret] = '\0';
		stash = ft_strjoin_gnl(stash, buf);
		if (!stash)
			return (NULL);
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[FD_MAX];
	char		*buf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FD_MAX)
		return (NULL);
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (free_stash(&stash[fd]));
	stash[fd] = read_stash(fd, stash[fd], buf);
	free(buf);
	if (!stash[fd] || !stash[fd][0])
		return (free_stash(&stash[fd]));
	line = ft_extract_line(stash[fd]);
	if (!line)
		return (free_stash(&stash[fd]));
	stash[fd] = ft_keep_rest(stash[fd]);
	return (line);
}
