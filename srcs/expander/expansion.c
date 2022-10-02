//void	exp_dollar(char *str, int i)
//{
//	while (str[i])
//	{
//		
//	}
//}
//
//void	replace_dollar(str, i)
//{
//	char	*s;
//	char	*exp_str;
//	char	*ret;
//
//	s = ft_substr(str, 0, i);
//	exp_str = exp_dollar(str, i);
//	ret = ft_strjoin(s, exp_dollar);
//}

char	*handle_single_quote(char *str, int *i)
{
//	char	*quote_next;
	char	*expanded_str;
	char	*in_quote_str;
//	char	*prev_str;
	char	*tmp;
	int		quote_head_index;
	int	j;

//	prev_str
	tmp	= ft_substr(str, 0, i);
	quote_head_index = i + 1;
	j = 0;
	while (str[++i] != '\'')
	{
		j++;
	}
	in_quote_str = ft_substr(str, quote_head_index, j);
	expanded_str = ft_strjoin(tmp, in_quote_str);
	free(tmp);
	free(in_quote_str);
	if (str[i + 1])
	{
		tmp = expanded_str;
		expanded_str = ft_strjoin(expanded_str, str + i + 1);
		free(tmp);
//		tmp = ft_strdup(str + i + 1);
	}
//	quote_next = ft_strchr(str + i, '\'');
	return (expanded_str);
}

char	*handle_double_quote(char *str, int	*i)
{
	char	*tmp;
	char	*in_quote_str;
	
	tmp	= ft_substr(str, 0, i);
	quote_head_index = i + 1;
//	ft_strchr(str, '\"')
	while (str[++i] != '\"')
	{
		if (str[i] == '$')
		{
			free(tmp);
			tmp = ft_substr(str, 0, i);
			1handle_dollar();
		}
		j++;
	}
}

echo "abc$HOME abc $HOME def"

void	expansion()
{
	char	*expanded_str;
	char	*
	int		i;

	while (str[i])
	{
		if (str[i] == '\'')
		{
			tmp = str;
			str = handle_single_quote(str, i);
			free(tmp);
		}
		if (str[i] == '\"')
		{
			tmp = str;
			str = handle_double_quote();
			free(tmp);
		}
		if (str[i] == '&' && str[i + 1] && !is_space(str[i + 1]))
		{
			tmp = str;
			str = handle_dollar(str);
			free(tmp);
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
