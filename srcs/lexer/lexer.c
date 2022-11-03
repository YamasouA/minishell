#include "minishell.h"

bool	ft_isspace(char str)
{
	return (str == '\t' || str == ' ');
}

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

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	head = head->next;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

t_token	*tokenize_error(char token, t_token *head)
{
	g_sh_var.exit_status = 258;
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putchar_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	free_token_list(head);
	return (NULL);
}

t_token	*quote_error(char *token, t_token *head)
{
	char	*tmp;

	g_sh_var.exit_status = 258;
	ft_putstr_fd("minishell: unexpected EOF while looking for matching `", 2);
	while (*token != '\0')
	{
		if (*token == '\'' || *token == '"')
		{
			tmp = token;
			token = find_quote(token + 1, *token);
			if (token == NULL)
			{
				ft_putchar_fd(tmp[0], 2);
				break ;
			}
		}
		token++;
	}
	ft_putstr_fd("'\n", 2);
	free_token_list(head);
	return (NULL);
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
