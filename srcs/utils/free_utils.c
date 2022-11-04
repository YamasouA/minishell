#include "minishell.h"

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
		free(strs[i++]);
	free(strs);
}

void	all_free(char *line, t_token *tok, t_node *node)
{
	free(line);
	free_token(tok);
	free_node(node);
}
