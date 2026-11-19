/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfaris <cfaris@student.1337.ma>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-11-18 18:16:29 by cfaris            #+#    #+#             */
/*   Updated: 2026-11-18 18:16:29 by cfaris           ###   ########.1337     */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE
#define GET_NEXT_LINE

#include <stdlib.h>
#include <unistd.h>

size_t ft_strlen_gnl(char *str);
size_t ft_strchr_gnl(char *str);
char *ft_strjoin_gnl(char *dst, char *buf);

#endif

