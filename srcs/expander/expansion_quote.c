/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:53:35 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:53:36 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_s_quote(char *str, int *i)
{
	char	*in_quote_str;
	int		quote_head_index;
	int		j;

	quote_head_index = *i + 1;
	j = 0;
	while (str[++(*i)] != '\'')
	{
		j++;
	}
	in_quote_str = ft_substr(str, quote_head_index, j);
	(*i)++;
	return (in_quote_str);
}

char	*handle_d_quote(char *str, int *i, bool here_doc)
{
	char	*s;
	ssize_t	j;

	s = ft_strdup("");
	j = *i + 1;
	while (str[++(*i)] != '\"' && str[(*i)])
	{
		if (str[*i] == '$' && !here_doc)
		{
			s = ft_joinfree(s, ft_substr(str, j, *i - j));
			s = expand_dollar(str, s, i);
			j = *i;
			(*i)--;
		}
	}
	if (str[*i])
		s = ft_joinfree(s, ft_substr(str, j, *i - j));
	(*i)++;
	return (s);
}
