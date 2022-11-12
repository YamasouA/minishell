/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta <asouta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:51:30 by shongou           #+#    #+#             */
/*   Updated: 2022/11/05 23:40:11 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	echo_without_newline(char **strs, size_t i)
{
	while (strs[i])
	{
		ft_putstr_fd(strs[i++], STDOUT_FILENO);
		if (strs[i] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
}

static void	echo_with_option_n(char **strs)
{
	size_t	i;

	i = 2;
	echo_without_newline(strs, i);
}

static void	echo_no_option(char **strs)
{
	size_t	i;

	i = 1;
	echo_without_newline(strs, i);
	ft_putstr_fd("\n", STDOUT_FILENO);
}

int	ft_echo(char **strs)
{
	if (strs[1] == NULL)
		ft_putstr_fd("\n", STDOUT_FILENO);
	else if (ft_strlen(strs[1]) == 2 && ft_strncmp(strs[1], "-n", 3) == 0)
	{
		echo_with_option_n(strs);
	}
	else
	{
		echo_no_option(strs);
	}
	return (0);
}
