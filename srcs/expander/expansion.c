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

void	split_free(char **ptr_ptr)
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

/*
char	*find_env(char *var, size_t len)
{
	size_t	i;
	char	*exp_var;
	char	**split;

	i = 0;
	while (envp[i] != NULL)
	{
		split = ft_split(envp[i], "=");
		if (split == NULL)
			return (NULL);
		if (ft_strlen(split == len) && ft_strncmp(var, split[0], len) == 0)
		{
			exp_var = split[1];
			free_split(split);
			return (exp_var);
		}
		i++;
	}
	return (ft_strdup(""));
}
*/
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
	char	*tmp;
	size_t	j;
	char	*in_quote_str;
	
//	s	= ft_substr(str, 0, i);
	s = ft_strdup("");
	j = *i + 1;
	while (str[++(*i)] != '\"')
	{
		if (str[*i] == '$')
		{
			s = ft_joinfree(s, ft_substr(str[], ));
			s = ft_joinfree(s, handle_dollar(str[*i], i));
			j = i;
		}
	}
	if (j != *i - 1)
		s = ft_joinfree(s, ft_substr(str[], 0, *i - 1 - j));
	return (s);
}

// "HOME abcdefg"
// "HOME $HOMEabc"
// "abc$HOME abcd"

echo "abc$HOME abc $HOME def"

void	expansion()
{
	char	*expanded_str;
	char	*
	int		i;

	expanded_str = ft_strdup("");
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
		if (str[i] == '$' && str[i + 1] && !is_space(str[i + 1]))
		{
			tmp = str;
			str = handle_dollar(str, &i);
			free(tmp);
		}
		else
		{
			head_index = i
			while (str[i] && str[i] != '\'' && str[i] != '\"' && str[i] != '$')
				i++;
			expanded_str = ft_joinfree(expanded_str, ft_substr(str, head_index, i - head_index));
		}
	}

	//if double_quote
	//	handle_double_quote
	//		if $
	//			handle_dollar
	//else if single_quote
	//	handle_single_quote
	//else if $ && next != '\0' && next != isspace
	//	handle_dollar	


//	int	status;
//
//	status = GENERAL;
//	while (str[i])
//	{
//		if (str[i] == '\"' && status == GENERAL)
//		{
//			handle_d_quote
////			status = D_QUOTE;
//		}
////		else if (str[i] == '\"' && status == D_QUOTE)
////			status = GENERAL;
//		if (str[i] == "\'" && status == GENERAL)
//			status = S_QUOTE;
//		else if (str[i] == "\'" && status == S_QUOTE)
//			status = GENERAL;
//		if (str[i] == '$' && str[i + 1] != '\0' && ft_strchr(" \t\r\v", str[i + 1]) && status != S_QUOTE)
//		{
//			replace_dollar(str, i++);
//			
//		}
//	}
}

void	expansion(t_node *node)
{
	int	i;

	i = 0;
	if (node->lhs == NULL && node->rhs == NULL)
	{
		if (node->cmd->cmd == NULL)
			return ;
		while (node->cmd->cmd[i] != NULL)
		{
			if (ft_strchr(node->cmd->cmd[i], '$') || ft_strchr(node->cmd->cmd[i], '\'') || ft_strchr(node->cmd->cmd[i], '\"'))
				tmp = ndoe->cmd->cmd[i];
				node->cmd->cmd[i] = expand();
				free(tmp);
		}
		while (node->cmd->redirect_in->next)
		{
			node->cmd->redirect_in = node->cmd->redirect_in->next;
			if (node->cmd->redirect_in->type == HEREDOC)
				node->cmd->redirect_in->delemiter;
			else
				node->cmd->redirect_in->file_name;
		}
		while (node->cmd->redirect_out->next)
		{
			node->cmd->redirect_out = node->cmd->redirect_out->next;
			node->cmd->redirect_out->file_name;
		}
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			expansion(node->lhs);
		if (node->rhs != NULL)
			expansion(node->rhs);
	}
}
