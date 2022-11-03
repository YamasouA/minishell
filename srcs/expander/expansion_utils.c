#include "minishell.h"

size_t	strptr_len(char **line)
{
	size_t	cnt;

	cnt = 0;
	if (line == NULL)
		return (0);
	while (*line++ != NULL)
		cnt++;
	return (cnt);
}

void	free_split(char **ptr_ptr) //similar func exist ?
{
	size_t	len;
	size_t	i;

	if (ptr_ptr == NULL)
		return ;
	len = strptr_len(ptr_ptr);
	i = 0;
	while (i < len)
		free(ptr_ptr[i++]);
	free(ptr_ptr);
}

bool	is_expand_none(char *str, char *expanded, int i)
{
	if (expanded[0] == '\0' && str[i] == '\0')
	{
		free(expanded);
		return (true);
	}
	return (false);
}
