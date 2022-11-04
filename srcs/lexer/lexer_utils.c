/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:54:10 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:54:12 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_isspace(char str)
{
	return (str == '\t' || str == ' ');
}

bool	is_meta(char c)
{
	return (ft_strchr(" \t|<>", c));
}

size_t	len_keyword(char *c)
{
	static char	*kw[] = {"|", ">>", "<<", "<", ">"};
	static char	*kw2[] = {"export"};
	int			i;

	i = 0;
	while (i < 5)
	{
		if (ft_strncmp(kw[i], c, ft_strlen(kw[i])) == 0)
			return (ft_strlen(kw[i]));
		i++;
	}
	i = 0;
	while (i < 1)
	{
		if (ft_strncmp(kw2[i], c, ft_strlen(kw2[i])) == 0 \
			&& !ft_isalnum(c[ft_strlen(kw2[i])]))
			return (ft_strlen(kw2[i]));
		i++;
	}
	return (0);
}

char	*find_quote(char *line, char quote)
{
	while (*line != '\0')
	{
		if (*line == quote)
			return (line);
		line++;
	}
	return (NULL);
}

ssize_t	len_word(char *line)
{
	char	*tmp;

	tmp = line;
	while (*tmp != '\0' && !is_meta(*tmp))
	{
		if (*tmp == '\'' || *tmp == '"')
		{
			tmp = find_quote(tmp + 1, *tmp);
			if (tmp == NULL)
				return (-1);
		}
		tmp++;
	}
	return (tmp - line);
}
