/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta <asouta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 22:27:30 by asouta            #+#    #+#             */
/*   Updated: 2022/04/21 21:26:31 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*split(char **before, char c)
{
	char	*pos;
	char	*line;
	size_t	line_len;
	char	*temp;

	pos = ft_strchr(*before, c);
	line_len = pos - *before + 1;
	line = ft_strdup(*before);
	temp = *before;
	if (c == '\0')
		line_len -= 1;
	*before = ft_strdup(*before + line_len);
	free(temp);
	if (line == NULL || *before == NULL)
	{
		free(before);
		free(line);
		before = NULL;
		line = NULL;
	}
	return (line);
}

static void	read_size(char **before, int fd)
{
	ssize_t	cnt;
	char	*temp;
	char	*buf;

	cnt = 1;
	while (cnt > 0 && ft_strchr(*before, '\n') == NULL)
	{
		buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (buf == NULL)
			return ;
		cnt = read(fd, buf, BUFFER_SIZE);
		if (cnt <= 0)
			free(buf);
		else
		{
			buf[cnt] = '\0';
			temp = *before;
			*before = ft_strjoin(*before, buf);
			free(temp);
			free(buf);
			if (*before == NULL)
				return ;
		}
	}
}

char	*get_next_line(int fd)
{
	static char		*before = NULL;

	if (fd < 0 || fd > 256 || BUFFER_SIZE <= 0)
		return (NULL);
	if (before == NULL)
	{
		before = ft_strdup("");
		if (before == NULL)
			return (NULL);
	}
	read_size(&before, fd);
	if (ft_strchr(before, '\n'))
		return (split(&before, '\n'));
	else if (ft_strlen(before) > 0)
		return (split(&before, '\0'));
	free(before);
	before = NULL;
	return (NULL);
}
