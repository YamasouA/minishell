#include "minishell.h"

char	*handle_single_quote(char *str, int *i)
{
//	char	*quote_next;
	char	*in_quote_str;
//	char	*prev_str;
	int		quote_head_index;
	int	j;

//	prev_str
//	tmp	= ft_substr(str, 0, i);
	quote_head_index = *i + 1;
	j = 0;
	while (str[++(*i)] != '\'')
	{
		j++;
	}
	in_quote_str = ft_substr(str, quote_head_index, j);
//	expanded_str = ft_joinfree(tmp, in_quote_str);
//	free(tmp);
//	free(in_quote_str);
//	if (str[i + 1])
//	{
//		tmp = expanded_str;
//		expanded_str = ft_strjoin(expanded_str, str + i + 1);
//		free(tmp);
////		tmp = ft_strdup(str + i + 1);
//	}
//	quote_next = ft_strchr(str + i, '\'');
	return (in_quote_str);
//	return (expanded_str);
}

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

void	free_split(char **ptr_ptr)
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


char	*find_env(char *var, size_t len)
{
	size_t	i;
	char	*exp_var;
	char	**split;

	i = 0;
//	printf("len: %zu\n", len);
//	printf("var: %s\n", var);
	while (environ[i] != NULL)
	{
		//printf("env[%zu]: %s\n", i, environ[i]);
		split = ft_split(environ[i], '=');
		if (split == NULL)
			return (NULL);
		if (ft_strlen(split[0]) == len && ft_strncmp(var, split[0], len) == 0)
		{
			exp_var = ft_strdup(split[1]);
			free_split(split);
//			printf("exp_var: %s\n", exp_var);
			return (exp_var);
		}
		i++;
	}
	return (ft_strdup(""));
}

bool	ft_isspace2(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\r');
}

char	*handle_dollar(char *str, int *i)
{
	char	*var;
	int	j;
	
	(*i)++;
	if (str[*i] == '\'' || str[*i] == '\"')
	{
		return (ft_strdup(""));
	}
	j = *i;
	//write(1, "abc", 3);
//	printf("bef: %d\n", *i);
	while (str[j] != '$' && str[j] && !ft_isspace2(str[j]) && str[j] != '\'' && str[j] != '\"')
		j++;
	var = find_env(&str[*i], j - *i);
//	printf("var: %s\n", var);
	//var = ft_strdup("");
	*i = j;
//	printf("aft: %d\n", *i);
	return (var);
}

//char	*ft_substr2(char const *s, unsigned int start, size_t len)
//{
//	size_t	i;
//	char	*ret;
//
//	if (!s)
//		return (NULL);
//	if (start >= ft_strlen(s) || !len)
//		return (ft_strdup(""));
//	if (len > ft_strlen(s + start))
//		ret = (char *)malloc((ft_strlen(s + start) + 1) * sizeof(char));
//	else
//		ret = (char *)malloc(len + 1);
//	if (!ret)
//		return (NULL);
//	i = 0;
//	while (i < len && s[start + i])
//	{
//		ret[i] = s[start + i];
//		i++;
//	}
//	ret[i] = '\0';
//	return (ret);
//}

char	*handle_double_quote(char *str, int *i)
{
	char	*s;
//	char	*tmp;
	ssize_t	j;
//	char	*in_quote_str;
	
//	s	= ft_substr(str, 0, i);
	s = ft_strdup("");
	j = *i + 1;
	while (str[++(*i)] != '\"' && str[(*i)])
	{
		if (str[*i] == '$')
		{
//			printf("%s\n", str);
//			printf("%s\n", ft_substr(str, *i, 4));
			s = ft_joinfree(s, ft_substr(str, j, *i - j));
//			printf("%s\n", s);
//			printf("%s\n", handle_dollar(str, i));
			s = ft_joinfree(s, handle_dollar(str, i));
//			printf("%s\n", s);
			j = *i;
			(*i)--;
		}
	}
//	printf("%s\n", ft_substr(str, j, *i - j));
//	if (j != *i - 1)
	if (str[*i])
		s = ft_joinfree(s, ft_substr(str, j, *i - j));
//	printf("ok\n");
//	printf("%s\n", s);
	(*i)++;
	return (s);
}

// "HOME abcdefg"
// "HOME $HOMEabc"
// "abc$HOME abcd"

//echo "abc$HOME abc $HOME def"

char	*expand(char *str)
{
	char	*expanded_str;
//	char	*tmp;
	int		i;
	int		head_index;

	expanded_str = ft_strdup("");
	i = 0;
	while (str[i])
	{
		//printf("str[%d]: %c\n", i, str[i]);
		if (str[i] == '\'')
		{
			//tmp = expanded_str;
			expanded_str = ft_joinfree(expanded_str, handle_single_quote(str, &i));
//			free(tmp);
		}
		if (str[i] == '\"')
		{
//			tmp = str;
//			printf("ok2\n");
			expanded_str = ft_joinfree(expanded_str, handle_double_quote(str, &i));
//			free(tmp);
		}
		if (str[i] == '$')
		{
			if (str[i + 1] && !isspace(str[i + 1]) && str[i + 1] != '$')
			{
//				tmp = expanded_str;
				expanded_str = ft_joinfree(expanded_str, handle_dollar(str, &i));
				printf("ok\n");
//				free(tmp);
			}
			else
			{
				expanded_str = ft_joinfree(expanded_str, ft_strdup("$"));
				i++;
			}
		}
		else
		{
			head_index = i;
			while (str[i] && str[i] != '\'' && str[i] != '\"' && str[i] != '$')
				i++;
			expanded_str = ft_joinfree(expanded_str, ft_substr(str, head_index, i - head_index));
//			i++;
		}
	}
	return (expanded_str);
	//if double_quote
	//	handle_double_quote
	//		if $
	//			handle_dollar
	//else if single_quote
	//	handle_single_quote
	//else if $ && next != '\0' && next != isspace
	//	handle_dollar	
}

t_node	*expansion(t_node *node)
{
	char	*tmp;
	int		i;
	t_redirect	*head;
//	t_redirect	*out_head;

	i = 0;
	if (node->lhs == NULL && node->rhs == NULL)
	{
		if (node->cmd->cmd == NULL)
			return (NULL);
		while (node->cmd->cmd[i] != NULL)
		{
			if (ft_strchr(node->cmd->cmd[i], '$') || ft_strchr(node->cmd->cmd[i], '\'') || ft_strchr(node->cmd->cmd[i], '\"'))
			{
				tmp = node->cmd->cmd[i];
				node->cmd->cmd[i] = expand(node->cmd->cmd[i]);
//				printf("%s\n", node->cmd->cmd[i]);
				free(tmp);
			}
			i++;
		}
		head = node->cmd->redirect_in;
		while (node->cmd->redirect_in->next)
		{
			node->cmd->redirect_in = node->cmd->redirect_in->next;
			if (node->cmd->redirect_in->type == HEREDOC)
			{
				if (ft_strchr(node->cmd->redirect_in->delemiter, '$') || ft_strchr(node->cmd->redirect_in->delemiter, '\'')
					|| ft_strchr(node->cmd->redirect_in->delemiter, '\"'))
				{
					tmp = node->cmd->redirect_in->delemiter;
					node->cmd->redirect_in->delemiter = expand(node->cmd->redirect_in->delemiter);
					printf("%s\n", node->cmd->redirect_in->delemiter);
					free(tmp);
				}
			}
			else
			{
				if (ft_strchr(node->cmd->redirect_in->file_name, '$') || ft_strchr(node->cmd->redirect_in->file_name, '\'')
					|| ft_strchr(node->cmd->redirect_in->file_name, '\"'))
				{
					tmp = node->cmd->redirect_in->file_name;
					node->cmd->redirect_in->file_name = expand(node->cmd->redirect_in->file_name);
					printf("%s\n", node->cmd->redirect_in->file_name);
					free(tmp);
				}
			}
		}
		node->cmd->redirect_in = head;
//		free(in_head);
		head = node->cmd->redirect_out;
		while (node->cmd->redirect_out->next)
		{
			node->cmd->redirect_out = node->cmd->redirect_out->next;
			if (ft_strchr(node->cmd->redirect_out->file_name, '$') || ft_strchr(node->cmd->redirect_out->file_name, '\'')
				|| ft_strchr(node->cmd->redirect_out->file_name, '\"'))
			{
				tmp = node->cmd->redirect_out->file_name;
				node->cmd->redirect_out->file_name = expand(node->cmd->redirect_out->file_name);
				printf("%s\n", node->cmd->redirect_out->file_name);
				free(tmp);
			}
		}
		node->cmd->redirect_out = head;
//		free(out_head);
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			expansion(node->lhs);
		if (node->rhs != NULL)
			expansion(node->rhs);
	}
	return (node);
}
