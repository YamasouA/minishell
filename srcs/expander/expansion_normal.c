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

