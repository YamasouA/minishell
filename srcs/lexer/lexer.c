#include "minishell.h"

bool	ft_isspace(char *str)
{
	return (*str == '\t' || *str == ' ');
}

t_token	*create_token(t_kind kind, char *c, size_t len)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->kind = kind;
	token->str = c;
	token->len = len;
	token->next = NULL;
	return (token);
}

bool	is_keyword(char c)
{
	return (strchr(" \t|&;()<>", c));
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

void	print_list(t_token *list)
{
	static char	*kind[4] = {"TK_KEYWORD", "TK_STR", "TK_IDENT", "TK_HEAD"};
	size_t		i;

	while (list != NULL)
	{
		printf("ENUM: %s\n", kind[list->kind]);
		printf("str: ");
		i = 0;
		while (i < list->len)
		{
			printf("%c", *(list->str + i));
			i++;
		}
		printf("\n");
		printf("len: %zu\n", list->len);
		printf("\n");
		list = list->next;
	}
}

char	*find_quote(char *line, char quote)
{
	while (*line != '\0')
	{
		if (*line == quote && *(line - 1) != '\\')
			return (line);
		line++;
	}
	return (NULL);
}

ssize_t	len_word(char *line)
{
	char	*tmp;

	tmp = line;
	while (*tmp != '\0' && !is_keyword(*tmp))
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

void	tokenize(t_token *cur, char *line)
{
	ssize_t	len;

	while (*line != '\0')
	{
		if (ft_isspace(line))
		{
			line++;
			continue ;
		}
		len = len_keyword(line);
		if (len > 0)
			cur->next = create_token(TK_KEYWORD, line, len);
		else if (!is_keyword(*line))
		{
			len = len_word(line);
			if (len == -1)
				ft_exit2("error");
			cur->next = create_token(TK_STR, line, len);
		}
		line += len;
		if (cur->next == NULL)
			ft_exit2("error");
		cur = cur->next;
	}
}

t_token	*lexer(char *line)
{
	t_token	*head;
	t_token	*cur;
	//ssize_t	len;

	head = create_token(TK_HEAD, "", 0);
	cur = head;
	tokenize(cur, line);
	/*
	while (*line != '\0')
	{
		if (ft_isspace(line)) 
		{
			line++;
			continue;
		}
		len = len_keyword(line);
		if (len > 0)
		{
			cur->next = create_token(TK_KEYWORD, line, len);
			line += len;
		}
		else if (!is_keyword(*line))
		{
			len = len_word(line);
			if (len == -1)
				ft_exit("hello");
			cur->next = create_token(TK_STR, line, len);
			line += len;
		}
		if (cur->next == NULL)
			printf("error");
		cur = cur->next;
	}*/
//	print_list(head);
	return (head->next);
}
