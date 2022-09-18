/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hextoi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta </var/mail/asouta>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 15:51:49 by asouta            #+#    #+#             */
/*   Updated: 2021/11/03 16:06:52 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/libft.h"
#include "stdio.h"

bool	is_overflow(int sign, int num, int add, int base)
{
	if (sign == 1)
	{
		if (add <= (INT_MAX - num) / base)
			return (false);
	}
	else if (sign == -1)
	{
		if ((INT_MIN - num) / base <= add * sign)
			return (false);
	}
	return (true);
}

bool	is_hex(char c)
{
	char	*hex;
	size_t	cnt;

	hex = "0123456789abcdef";
	cnt = 0;
	while (hex[cnt] != '\0')
	{
		if (hex[cnt] == ft_tolower(c))
			return (true);
		cnt++;
	}
	return (false);
}

int	hextoi(char *str, int sign)
{
	size_t	idx;
	int		num;
	int		base_n;
	char	*base;

	idx = ft_strlen(str) - 1;
	num = 0;
	base = "0123456789abcdef";
	base_n = 1;
	while (is_hex(str[idx]))
	{
		if (is_overflow(sign, num,
				ft_searchidx(base, ft_tolower(str[idx])), base_n))
		{
			if (sign == 1)
				return (INT_MAX);
			return (INT_MIN);
		}
		num += sign * ft_searchidx(base, ft_tolower(str[idx])) * base_n;
		base_n *= 16;
		idx--;
	}
	return (num);
}

int	ft_hextoi(char *str)
{
	int		num;
	int		sign;

	sign = 1;
	if (*str == '-' || *str == '+')
		sign = 44 - *str++;
	if (str[0] != '0' || str[1] != 'x')
		return (0);
	str += 2;
	num = hextoi(str, sign);
	return (num);
}
