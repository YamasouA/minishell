/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xunlink.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:59:29 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:59:31 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	xunlink(const char *pathname)
{
	int	ret;

	ret = unlink(pathname);
	error_checker("unlink error", ret);
}
