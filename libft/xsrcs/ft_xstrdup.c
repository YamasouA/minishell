/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_xstrdup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:58:44 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:58:46 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_xstrdup(const char *s1)
{
	size_t	len;
	char	*sp;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	sp = (char *)malloc(len + 1);
	if (sp == NULL)
	{
		ft_putendl_fd("malloc error", 2);
		exit(1);
	}
	ft_strlcpy(sp, s1, len + 1);
	return (sp);
}
