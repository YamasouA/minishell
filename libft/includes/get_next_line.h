/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta <asouta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 22:31:18 by asouta            #+#    #+#             */
/*   Updated: 2022/04/14 19:04:34 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include	<unistd.h>
# include	<stdlib.h>
# include	"libft.h"
# define BUFFER_SIZE 1

char	*get_next_line(int fd);
//char	*ft_strchr(const char *s, int c);
//char	*ft_strjoin(char const *s1, char const *s2);
//size_t	ft_strlen(const char *str);
//size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
#endif
