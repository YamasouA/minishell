/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpipe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:59:23 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:59:25 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	xpipe(int pipefd[2])
{
	int	ret;

	ret = pipe(pipefd);
	error_checker("pipe error", ret);
}
