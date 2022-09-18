/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_searchidx.                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta </var/mail/asouta>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 15:51:49 by asouta            #+#    #+#             */
/*   Updated: 2021/11/03 16:06:52 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/libft.h"

size_t	ft_searchidx(char *line, char c)
{
	size_t	idx;
	size_t	len;

	if (line == NULL)
		return (0);
	idx = 0;
	len = ft_strlen(line);
	while (idx < len)
	{
		if (line[idx] == c)
			return (idx);
		idx++;
	}
	return (0);
}
