/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfaris <cfaris@student.1337.ma>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-11-21 16:16:32 by cfaris            #+#    #+#             */
/*   Updated: 2026-11-21 16:16:32 by cfaris           ###   ########.1337     */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

size_t	ft_strlen_gnl(char *str);
int		ft_strchr_gnl(char *str);
char	*ft_strjoin_gnl(char *dst, char *buf);
char	*ft_extract_line(char *stash);
char	*ft_keep_rest(char *stash);

char	*get_next_line(int fd);

#endif
