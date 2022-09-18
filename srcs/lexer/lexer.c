#include "minishell.h"

bool    ft_isspace(char *str)
{
	 return (('\t' <= *str && *str <= '\r') || *str == ' ');
}

//size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
//{
//	size_t	i;
//
//	i = 0;
//	if (dstsize == 0)
//		return (ft_strlen(src));
//	while (dstsize-- > 1 && src[i])
//	{
//		dst[i] = src[i];
//		i++;
//	}
//	dst[i] = '\0';
//	return (ft_strlen(src));
//}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	uc;
	unsigned char	*p;

	uc = (unsigned char)c;
	p = (unsigned char *)b;
	while (len-- > 0)
		*p++ = uc;
	return (b);
}

void	ft_bzero(void *s, size_t n)
{
	if (n == 0)
		return ;
	ft_memset(s, '\0', n);
	return ;
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*reserve_mem;

	if (!(count && size))
	{
		count = 1;
		size = 1;
	}
	if (count && (size > SIZE_MAX / count))
		return (NULL);
	reserve_mem = (void *)malloc(count * size);
	if (!reserve_mem)
		return (NULL);
	ft_bzero(reserve_mem, count * size);
    return (reserve_mem);
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

char *is_keyword(char *c)
{
    char *kw[] = {"|", ">>", "<<", "<", ">", "$"};
    char *kw2[] = {"export"};
    int i;

    i = 0;
    while (i < 6)
    {
        if (strncmp(kw[i],c, strlen(kw[i])) == 0)
            return (kw[i]);
        i++;
    }
    i = 0;
    while (i < 1)
    {
        if (strncmp(kw2[i], c, strlen(kw2[i])) == 0\
            && !isalnum(c[strlen(kw2[i])]))
            return (kw2[i]);
		i++;
     }
    return (NULL);
}

bool	is_not_keyword(char c)
{
	return (!strchr("|&;()<>", c));
}

void print_list(t_token *list)
{
    while (list != NULL)
    {
        printf("ENUM: %d\n", list->kind);
        printf("str: %s\n", list->str);
        printf("len: %zu\n", list->len);

        list = list->next;
    }
}

size_t	cmd_len(char *line)
{
	char *tmp;

	tmp = line;
//	while (isalnum(*tmp))
	while (*tmp &&!is_keyword(tmp) && !ft_isspace(tmp))
		tmp++;
	return tmp - line;
}

void lexer(char *line)
{
    //size_t  idx;
    t_token *head;
    t_token *cur;
    char *kw;

    head = create_token(TK_HEAD, "", 0);
    cur = head;
    while (*line != '\0')
    {
        if (ft_isspace(line)) 
		{
            line++;
            continue;
        }
        kw = is_keyword(line);
		if (kw)
		{
            cur->next = create_token(TK_KEYWORD, line, strlen(kw));
			line += strlen(kw);
		}
//		if (isalnum(*line))
		if (is_not_keyword(*line))
		{
			cur->next = create_token(TK_STR, line, cmd_len(line));
			line += cmd_len(line);
		}
		if (cur->next == NULL)
			printf("fuck");
        cur = cur->next;
    }
    print_list(head);

    // cur->next = create_token(line, idx);
}
