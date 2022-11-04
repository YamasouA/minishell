/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:53:46 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:53:48 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_expand_none(char *str, char *expanded, int i)
{
	if (expanded[0] == '\0' && str[i] == '\0')
	{
		free(expanded);
		return (true);
	}
	return (false);
}
