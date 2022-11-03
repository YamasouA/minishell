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
