/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:53:18 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:53:20 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*exp_exit_status(int *i)
{
	(*i)++;
	return (ft_strdup(ft_itoa(g_sh_var.exit_status)));
}

static int	is_odd_dquote(char *str)
{
	int		i;
	int		cnt;

	cnt = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
			cnt++;
		i++;
	}
	return (cnt % 2);
}

static char	*handle_dollar(char *str, int *i)
{
	char	*var;
	int		j;

	(*i)++;
	if (str[*i] == '\'' || str[*i] == '\"')
	{
		if (str[*i] == '\'' || is_odd_dquote(str + *i + 1))
			return (ft_strdup(""));
		else
			return (ft_strdup("$"));
	}
	j = *i;
	if (str[j] == '?')
		return (exp_exit_status(i));
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

char	*expand_dollar(char *str, char *expanded, int *i)
{
	if (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_'
		|| str[*i + 1] == '\'' || str[*i + 1] == '"' || str[*i + 1] == '?')
		expanded = ft_joinfree(expanded, handle_dollar(str, i));
	else
	{
		expanded = ft_joinfree(expanded, ft_strdup("$"));
		*i += 1;
	}
	return (expanded);
}
