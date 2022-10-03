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
	while (environ[i] != NULL)
	{
		split = ft_split(environ[i], '=');
		if (split == NULL)
			return (NULL);
		if (ft_strlen(split[0]) == len && ft_strncmp(var, split[0], len) == 0)
		{
			exp_var = split[1];
			free_split(split);
			return (exp_var);
		}
		i++;
	}
	return (ft_strdup(""));
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
	while (str[*i] != '$' && str[*i] && !ft_isspace(str))
		j++;
	//var = find_env(str[i], j - i);
	var = ft_strdup("");
	*i = j;
	return (var);
}

char	*handle_double_quote(char *str, int *i)
{
	char	*s;
//	char	*tmp;
	ssize_t	j;
//	char	*in_quote_str;
	
//	s	= ft_substr(str, 0, i);
	s = ft_strdup("");
	j = *i + 1;
	while (str[++(*i)] != '\"')
	{
		if (str[*i] == '$')
		{
			s = ft_joinfree(s, ft_substr(str, j, j - *i));
			s = ft_joinfree(s, handle_dollar(str, i));
			j = *i;
		}
	}
	if (j != *i - 1)
		s = ft_joinfree(s, ft_substr(str, j, *i - 1 - j));
	return (s);
}

// "HOME abcdefg"
// "HOME $HOMEabc"
// "abc$HOME abcd"

//echo "abc$HOME abc $HOME def"

char	*expand(char *str)
{
	char	*expanded_str;
	char	*tmp;
	int		i;
	int		head_index;

	expanded_str = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			//tmp = expanded_str;
			expanded_str = ft_joinfree(expanded_str, handle_single_quote(str, &i));
//			free(tmp);
		}
		if (str[i] == '\"')
		{
//			tmp = str;
			str = ft_joinfree(expanded_str, handle_double_quote(str, &i));
//			free(tmp);
		}
		if (str[i] == '$' && str[i + 1] && !isspace(str[i + 1]))
		{
			tmp = str;
			str = handle_dollar(str, &i);
			free(tmp);
		}
		else
		{
			head_index = i;
			while (str[i] && str[i] != '\'' && str[i] != '\"' && str[i] != '$')
				i++;
			expanded_str = ft_joinfree(expanded_str, ft_substr(str, head_index, i - head_index));
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
				printf("%s\n", node->cmd->cmd[i]);
				free(tmp);
			}
			i++;
		}
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
