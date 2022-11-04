/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xopen.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:59:17 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:59:19 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	xopen(char *path, int flags)
{
	int	fd;

	fd = open(path, flags);
	error_checker("open error", fd);
	return (fd);
}
