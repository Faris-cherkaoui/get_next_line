/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfaris <cfaris@student.1337.ma>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-11-18 18:17:39 by cfaris            #+#    #+#             */
/*   Updated: 2026-11-18 18:17:39 by cfaris           ###   ########.1337     */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen_gnl(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strchr_gnl(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strjoin_gnl(char *dst, char *buf)
{
	char	*db;
	size_t	i;
	size_t	j;

	db = malloc(sizeof(char) * (ft_strlen_gnl(dst) + ft_strlen_gnl(buf) + 1));
	if (!db)
	{
		free(dst);
		return (NULL);
	}
	i = 0;
	while (dst && dst[i])
	{
		db[i] = dst[i];
		i++;
	}
	j = 0;
	while (buf[j])
	{
		db[i + j] = buf[j];
		j++;
	}
	db[i + j] = '\0';
	free(dst);
	return (db);
}

char	*ft_extract_line(char *stash)
{
	int		i;
	int		len;
	char	*line;

	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	if (stash[len] == '\n')
		len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_keep_rest(char *stash)
{
	char	*rest;
	int		i;
	int		j;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	rest = NULL;
	if (stash[i] != '\0')
		rest = malloc(sizeof(char) * (ft_strlen_gnl(stash) - i + 1));
	j = 0;
	while (rest && stash[i + j])
	{
		rest[j] = stash[i + j];
		j++;
	}
	if (rest)
		rest[j] = '\0';
	free(stash);
	return (rest);
}
