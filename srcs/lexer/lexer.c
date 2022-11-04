#include "minishell.h"

static t_token	*create_token(t_kind kind, char *c, size_t len)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		err_exit("malloc error: ");
	token->kind = kind;
	token->str = c;
	token->len = len;
	token->next = NULL;
	return (token);
}

static t_token	*tokenize(t_token *cur, char *line, t_token *head)
{
	ssize_t	len;

	while (*line != '\0')
	{
		if (ft_isspace(*line))
		{
			line++;
			continue ;
		}
		len = len_keyword(line);
		if (len > 0)
			cur->next = create_token(TK_KEYWORD, line, len);
		else if (!is_meta(*line))
		{
			len = len_word(line);
			if (len == -1)
				return (quote_error(line, head));
			cur->next = create_token(TK_STR, line, len);
		}
		line += len;
		if (cur->next == NULL)
			return (tokenize_error(*line, head));
		cur = cur->next;
	}
	return (head);
}

t_token	*lexer(char *line)
{
	t_token	head;
	t_token	*cur;

	cur = &head;
	cur->next = NULL;
	if (!tokenize(cur, line, &head))
		return (NULL);
	return (head.next);
}
