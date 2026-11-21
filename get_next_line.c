/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfaris <cfaris@student.1337.ma>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-11-19 10:04:29 by cfaris            #+#    #+#             */
/*   Updated: 2026-11-19 10:04:29 by cfaris           ###   ########.1337     */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	static char	*stash;
	char		*buf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (free_stash(&stash));
	stash = read_stash(fd, stash, buf);
	free(buf);
	if (!stash || !stash[0])
		return (free_stash(&stash));
	line = ft_extract_line(stash);
	if (!line)
		return (free_stash(&stash));
	stash = ft_keep_rest(stash);
	return (line);
}
