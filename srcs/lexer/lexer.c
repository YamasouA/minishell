#include "minishell.h"


//void	print_list(t_token *list)
//{
//	static char	*kind[4] = {"TK_KEYWORD", "TK_STR", "TK_IDENT", "TK_HEAD"};
//	size_t		i;
//
//	while (list != NULL)
//	{
//		printf("ENUM: %s\n", kind[list->kind]);
//		printf("str: ");
//		i = 0;
//		while (i < list->len)
//		{
//			printf("%c", *(list->str + i));
//			i++;
//		}
//		printf("\n");
//		printf("len: %zu\n", list->len);
//		printf("\n");
//		list = list->next;
//	}
//}


//void	free_token_list(t_token *head) //same token_free?
//{
//	t_token	*tmp;
//
//	head = head->next;
//	while (head != NULL)
//	{
//		tmp = head;
//		head = head->next;
//		free(tmp);
//	}
//}

t_token	*create_token(t_kind kind, char *c, size_t len)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		//return NULL;
		err_exit("malloc error: ");
	token->kind = kind;
	token->str = c;
	token->len = len;
	token->next = NULL;
	return (token);
}

t_token	*tokenize(t_token *cur, char *line, t_token *head)
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
