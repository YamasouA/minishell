#include "minishell.h"

bool	peek(t_token *tok, char *op)
{
	if (tok->kind != TK_KEYWORD
		|| tok->len != ft_strlen(op)
		|| ft_memcmp(tok->str, op, tok->len))
		return (false);
	return (true);
}

bool	consume(t_token **tok, char *op)
{
	if (!peek(*tok, op))
		return (false);
	*tok = (*tok)->next;
	return (true);
}

size_t	cmd_len(t_token *tok)
{
	size_t	n;

	n = 0;
	while (tok != NULL && !peek(tok, "|"))
	{
		n++;
		tok = tok->next;
	}
	return (n);
}
