/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xdup.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:58:59 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:59:01 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	xdup(int oldfd)
{
	int	ret;

	ret = dup(oldfd);
	error_checker("dup error", ret);
	return (ret);
}

void	xdup2(int oldfd, int newfd)
{
	int	ret;

	ret = dup2(oldfd, newfd);
	error_checker("dup2 error", ret);
}
