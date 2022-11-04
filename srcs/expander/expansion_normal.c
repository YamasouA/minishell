/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_normal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:53:30 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:53:31 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_normal(char *str, int *i, bool heredoc)
{
	int	head;

	head = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '\"'
		&& (str[*i] != '$' || heredoc))
		(*i)++;
	return (ft_substr(str, head, *i - head));
}
