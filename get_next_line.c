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

char *get_next_line(int fd)
{
    static char *carnet; // Une variable static est automatiquement initialisée à 0
    char *buf;
    int Ret;
    Ret = 1;
    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
    if(!buf)
        return (NULL);
    while(ft_strchr_gnl(carnet) && Ret > 0)
    {
        Ret = read(fd, buf, BUFFER_SIZE);
        if (Ret == -1)
        {
            free(carnet);
            free(buf);
            carnet = NULL;
            return(NULL)
        }
        buf[Ret] = '\0';
        carnet = ft_strjoin_gnl(carnet, buf);
        if(!carnet)
        {
            free (buf);
            return (NULL);
        }
    }
}        