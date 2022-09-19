#include "minishell.h"

bool    ft_isspace(char *str)
{
	 return (('\t' <= *str && *str <= '\r') || *str == ' ');
}


t_token *create_token(t_kind kind, char *c, size_t len)
{
    t_token *token;
    token = ft_calloc(1, sizeof(t_token));
    token->kind = kind;
	token->str = (char *)malloc(sizeof(char) * (len + 1));
    strlcpy(token->str, c, len + 1);
    token->len = len;
    token->next = NULL;
    return token;
}

bool	is_not_keyword(char c)
{
	return (!strchr(" \t|&;()<>", c));
}

size_t	is_keyword(char *c)
{
	char	*kw[] = {"|", ">>", "<<", "<", ">"};
	char	*kw2[] = {"export"};
	char	*kw3[] = {"$"};
	int		i;
	char	*tmp;

	i = 0;
	while (i < 5)
	{
		if (strncmp(kw[i], c, strlen(kw[i])) == 0)
			return (ft_strlen(kw[i]));
		i++;
	}
	i = 0;
	while (i < 1)
	{
		if (strncmp(kw2[i], c, strlen(kw2[i])) == 0\
			&& !isalnum(c[strlen(kw2[i])]))
			return (ft_strlen(kw2[i]));
		i++;
	 }
	 i = 0;
	 tmp = c;
	 while (i < 1)
	 {
	 	if (strncmp(kw3[i], c, strlen(kw3[i])) == 0\
			&& is_not_keyword(*(c + 1)))
		{
			while (is_not_keyword(*tmp))
				tmp++;
			return (tmp - c);
		}
		i++;
	}
	return (0);
}

void print_list(t_token *list)
{
	char *kind[4] = {"TK_KEYWORD", "TK_STR", "TK_IDENT", "TK_HEAD"};
	while (list != NULL)
	{
		printf("ENUM: %s\n", kind[list->kind]);
		printf("str: %s\n", list->str);
		printf("len: %zu\n", list->len);
		printf("\n");

		list = list->next;
	}
}

size_t	cmd_len(char *line)
{
	char *tmp;

	tmp = line;
	while (*tmp && is_not_keyword(*tmp))
		tmp++;
	return tmp - line;
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

void lexer(char *line)
{
	t_token *head;
	t_token *cur;
	size_t	len;
	char	*later_quote;

	head = create_token(TK_HEAD, "", 0);
	cur = head;
	while (*line != '\0')
	{
		if (ft_isspace(line)) 
		{
			line++;
			continue;
		}
		len = is_keyword(line);
		if (len > 0)
		{
			cur->next = create_token(TK_KEYWORD, line, len);
			line += len;
		}
		else if(*line == '\'' || *line == '\"')
		{
			later_quote = find_quote(line + 1, *line);
			if (later_quote == NULL)
				ft_exit("quote number is bad");
			later_quote += 1;
			cur->next = create_token(TK_STR, line, later_quote - line);
			line += (later_quote - line);
		}
		else if (is_not_keyword(*line))
		{
			cur->next = create_token(TK_STR, line, cmd_len(line));
			line += cmd_len(line);
		}
		if (cur->next == NULL)
			printf("error");
		cur = cur->next;
	}
	print_list(head);
}
