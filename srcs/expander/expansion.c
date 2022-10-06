#include "minishell.h"

char	*handle_single_quote(char *str, int *i)
{
	char	*in_quote_str;
	int		quote_head_index;
	int		j;

	quote_head_index = *i + 1;
	j = 0;
	while (str[++(*i)] != '\'')
	{
		j++;
	}
	in_quote_str = ft_substr(str, quote_head_index, j);
	(*i)++;
	return (in_quote_str);
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
			exp_var = ft_strdup(split[1]);
			free_split(split);
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
	int		j;

	(*i)++;
	if (str[*i] == '\'' || str[*i] == '\"')
	{
		return (ft_strdup(""));
	}
	j = *i;
	while (str[j] != '$' && str[j] && !ft_isspace2(str[j])
		&& str[j] != '\'' && str[j] != '\"')
		j++;
	var = find_env(&str[*i], j - *i);
	*i = j;
	return (var);
}

char	*handle_double_quote(char *str, int *i)
{
	char	*s;
	ssize_t	j;

	s = ft_strdup("");
	j = *i + 1;
	while (str[++(*i)] != '\"' && str[(*i)])
	{
		if (str[*i] == '$')
		{
			s = ft_joinfree(s, ft_substr(str, j, *i - j));
			s = ft_joinfree(s, handle_dollar(str, i));
			j = *i;
			(*i)--;
		}
	}
	if (str[*i])
		s = ft_joinfree(s, ft_substr(str, j, *i - j));
	(*i)++;
	return (s);
}

char	*expand_dollar(char *str, char *expanded, int *i)
{
	if (str[*i + 1] && !isspace(str[*i + 1]) && str[*i + 1] != '$')
		expanded = ft_joinfree(expanded, handle_dollar(str, i));
	else
	{
		expanded = ft_joinfree(expanded, ft_strdup("$"));
		*i += 1;
	}
	return (expanded);
}

char	*expand(char *str)
{
	char	*expanded;
	int		i;
	int		head;

	expanded = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			expanded = ft_joinfree(expanded, handle_single_quote(str, &i));
		else if (str[i] == '\"')
			expanded = ft_joinfree(expanded, handle_double_quote(str, &i));
		else if (str[i] == '$')
			expanded = expand_dollar(str, expanded, &i);
		else
		{
			head = i;
			while (str[i] && str[i] != '\'' && str[i] != '\"' && str[i] != '$')
				i++;
			expanded = ft_joinfree(expanded, ft_substr(str, head, i - head));
		}
	}
	return (expanded);
}

void expand_cmd_instance(char **cmd_data)
{
	char	*tmp;

	if (ft_strchr(*cmd_data, '\'')
		|| ft_strchr(*cmd_data, '\"')
		|| ft_strchr(*cmd_data, '$'))
	{
		tmp = *cmd_data;
		*cmd_data = expand(*cmd_data);
		free(tmp);
	}
}

t_node	*expansion(t_node *node)
{
//	char		*tmp;
	int			i;
	t_redirect	*head;

	if (node->lhs == NULL && node->rhs == NULL)
	{
		if (node->cmd->cmd == NULL)
			return (NULL);
		i = 0;
		while (node->cmd->cmd[i] != NULL)
		{
//			if (ft_strchr(node->cmd->cmd[i], '\'')
//				|| ft_strchr(node->cmd->cmd[i], '\"')
//				|| ft_strchr(node->cmd->cmd[i], '$'))
//			{
//				tmp = node->cmd->cmd[i];
			expand_cmd_instance(&(node->cmd->cmd[i]));
//				node->cmd->cmd[i] = expand(node->cmd->cmd[i]);
//				free(tmp);
//			}
			i++;
		}
		head = node->cmd->redirect_in;
		while (node->cmd->redirect_in->next)
		{
			node->cmd->redirect_in = node->cmd->redirect_in->next;
			if (node->cmd->redirect_in->type == HEREDOC)
			{
				expand_cmd_instance(&(node->cmd->redirect_in->delemiter));
				
//				if (ft_strchr(node->cmd->redirect_in->delemiter, '$')
//					|| ft_strchr(node->cmd->redirect_in->delemiter, '\'')
//					|| ft_strchr(node->cmd->redirect_in->delemiter, '\"'))
//				{
//					tmp = node->cmd->redirect_in->delemiter;
//					node->cmd->redirect_in->delemiter = expand(node->cmd->redirect_in->delemiter);
////					printf("%s\n", node->cmd->redirect_in->delemiter);
//					free(tmp);
//				}
			}
			else
			{
				expand_cmd_instance(&(node->cmd->redirect_in->file_name));
//				if (ft_strchr(node->cmd->redirect_in->file_name, '$')
//					|| ft_strchr(node->cmd->redirect_in->file_name, '\'')
//					|| ft_strchr(node->cmd->redirect_in->file_name, '\"'))
//				{
//					tmp = node->cmd->redirect_in->file_name;
////					node->cmd->redirect_in->file_name = expand(node->cmd->redirect_in->file_name);
////					printf("%s\n", node->cmd->redirect_in->file_name);
//					free(tmp);
//				}
			}
		}
		node->cmd->redirect_in = head;
		head = node->cmd->redirect_out;
		while (node->cmd->redirect_out->next)
		{
			node->cmd->redirect_out = node->cmd->redirect_out->next;
			expand_cmd_instance(&(node->cmd->redirect_out->file_name));
//			if (ft_strchr(node->cmd->redirect_out->file_name, '$')
//				|| ft_strchr(node->cmd->redirect_out->file_name, '\'')
//				|| ft_strchr(node->cmd->redirect_out->file_name, '\"'))
//			{
//				tmp = node->cmd->redirect_out->file_name;
//				node->cmd->redirect_out->file_name = expand(node->cmd->redirect_out->file_name);
////				printf("%s\n", node->cmd->redirect_out->file_name);
//				free(tmp);
//			}
		}
		node->cmd->redirect_out = head;
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
