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

size_t ft_strlen_gnl(char *str)
{
    size_t i;
    if(!str)
        return NULL;
    i = 0;
    while(str[i])
        i++;
    return(i);    
}
size_t ft_strchr_gnl(char *str)
{
    size_t i;
    if(!str)
        return 0;
    i = 0;
    while(str[i])
    {
        if(str[i] == '\n')
            return (1);
        i++;
    }
    return(0);
}
char *ft_strjoin_gnl(char *dst, char *buf)
{
    size_t d;
    size_t b;
    char *db;
    size_t i;
    size_t j;
    if(!dst || !buf)
        return(NULL);
    d = ft_strlen_gnl(dst);
    b = ft_strlen_gnl(buf);
    db = malloc(sizeof(char) * d + b + 1);
    if(!db)
        return(NULL);
    i = 0;
    while(i < d)
    {
        db[i] = dst[i];
        i++;
    }
    j = 0;
    while(i < b + d)
    {
        db[i] = buf[j];
        i++;
        j++;
    }    
    db[i] = '\0';
    return(db);
}
