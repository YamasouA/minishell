/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:53:24 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:53:25 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*exp_dollar(char *str, int *i)
{
	char	*var;
	int		j;

	*(i) += 1;
	if (!ft_isalnum(str[*i]))
		return (ft_strdup("$"));
	j = *i;
	if (ft_isalpha(str[j]) || str[j] == '_')
	{
		while (ft_isalnum(str[j]) || str[j] == '_')
			j++;
	}
	else if (ft_isdigit(str[j]))
		j++;
	var = find_env(&str[*i], j - *i);
	*i = j;
	return (var);
}

char	*expand_documents(char *str)
{
	int		i;
	int		head;
	char	*expanded;

	expanded = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			expanded = ft_joinfree(expanded, exp_dollar(str, &i));
		else
		{
			head = i;
			while (str[i] && str[i] != '$')
				i++;
			expanded = ft_joinfree(expanded, ft_substr(str, head, i - head));
		}
		if (expanded == NULL)
			err_exit("malloc error");
	}
	return (expanded);
}
