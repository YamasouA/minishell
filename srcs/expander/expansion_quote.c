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
