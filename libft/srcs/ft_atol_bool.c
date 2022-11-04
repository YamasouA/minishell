/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol_bool.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 23:02:34 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 23:02:36 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	bool	space(const char *str)
{
	if (*str == ' ' || *str == '\f' || *str == '\n'
		|| *str == '\r' || *str == '\t' || *str == '\v')
		return (true);
	else
		return (false);
}

static	bool	is_overflow(const char str, long long i, int sign)
{
	if (sign > 0 && (LLONG_MAX - sign * (str - '0')) / 10 >= i)
		return (false);
	else if (sign < 0 && (LLONG_MIN - sign * (str - '0')) / 10 <= i)
		return (false);
	return (true);
}

bool	ft_atol_bool(const char *str, long long *i)
{
	int		sign;

	*i = 0;
	sign = 1;
	while (space(str))
		str++;
	if (*str == '-' || *str == '+')
		sign = 44 - *str++;
	while (*str >= '0' && *str <= '9')
	{
		if (!is_overflow(*str, *i, sign))
			*i = sign * (*str - '0') + *i * 10;
		else
			return (false);
		str++;
	}
	return (true);
}

/*
#include <stdio.h>
int main()
{
	long long n;
	bool ret = ft_atol_bool("9223372036854775807", &n);
	printf("ret: %d\nn: %lld\n", ret, n);
	ret = ft_atol_bool("9223372036854775808", &n);
	printf("ret: %d\nn: %lld\n", ret, n);
	ret = ft_atol_bool("-9223372036854775808", &n);
	printf("ret: %d\nn: %lld\n", ret, n);
	ret = ft_atol_bool("-9223372036854775809", &n);
	printf("ret: %d\nn: %lld\n", ret, n);
	ret = ft_atol_bool("0", &n);
	printf("ret: %d\nn: %lld\n", ret, n);
}
*/
