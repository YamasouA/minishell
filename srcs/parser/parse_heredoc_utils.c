/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:54:31 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:54:32 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_digit(unsigned long n)
{
	int				count;
	unsigned long	num;

	count = 1;
	num = n;
	while (num > 9)
	{
		num = num / 10;
		count++;
	}
	return (count);
}

char	*ft_ultoa(unsigned long n)
{
	int				figure_len;
	char			*numstr;
	unsigned long	num;

	figure_len = count_digit(n);
	numstr = (char *)malloc(sizeof(char) * (figure_len + 1));
	if (!numstr)
		err_exit("malloc error: ");
	numstr[figure_len] = '\0';
	if (n == 0)
		numstr[0] = '0';
	num = n;
	while (num)
	{
		numstr[--figure_len] = (num % 10) + '0';
		num = num / 10;
	}
	return (numstr);
}

unsigned long	xorshift(void)
{
	static unsigned long	x = 123456789;
	static unsigned long	y = 362436069;
	static unsigned long	z = 521288629;
	static unsigned long	w = 88675123;
	unsigned long			t;

	t = (x ^ (x << 11));
	x = y;
	y = z;
	z = w;
	w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	return (w);
}
