#include "minishell.h"

char	*handle_s_quote(char *str, int *i)
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
	t_env	*tmp;

	tmp = g_sh_var.environ;
	while (tmp)
	{
		if (ft_strlen(tmp->key) == len
			&& ft_strncmp(tmp->key, var, len) == 0)
		{
			if (tmp->value != NULL)
				return (ft_strdup(tmp->value));
		}
		tmp = tmp->next;
	}
	return (ft_strdup(""));
}

char	*exp_exit_status(int *i)
{
	(*i)++;
	return (ft_strdup(ft_itoa(g_sh_var.exit_status)));
}

char	*handle_dollar(char *str, int *i)
{
	char	*var;
	int		j;

	(*i)++;
	if (str[*i] == '\'' || str[*i] == '\"')
	{
		if (str[*i] == '\'' || ft_strchr(str + *i + 1, '\"'))
			return (ft_strdup(""));
		else
			return (ft_strdup("$"));
	}
	j = *i;
	if (str[j] == '?')
		return (exp_exit_status(i));
	if (ft_isalpha(str[j]) || str[j] == '_')
	{
		while (ft_isalnum(str[j]) || str[j] == '_')
			j++;
	}
	else if (ft_isdigit(str[j]))
		j++;
	var = find_env(&str[*i], j - *i);
	*i = j;
	return (var);
}

char	*expand_dollar(char *str, char *expanded, int *i)
{
	if (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_'
		|| str[*i + 1] == '\'' || str[*i + 1] == '"' || str[*i + 1] == '?')
		expanded = ft_joinfree(expanded, handle_dollar(str, i));
	else
	{
		expanded = ft_joinfree(expanded, ft_strdup("$"));
		*i += 1;
	}
	return (expanded);
}

char	*handle_d_quote(char *str, int *i, bool here_doc)
{
	char	*s;
	ssize_t	j;

	s = ft_strdup("");
	j = *i + 1;
	while (str[++(*i)] != '\"' && str[(*i)])
	{
		if (str[*i] == '$' && !here_doc)
		{
			s = ft_joinfree(s, ft_substr(str, j, *i - j));
			s = expand_dollar(str, s, i);
			j = *i;
			(*i)--;
		}
	}
	if (str[*i])
		s = ft_joinfree(s, ft_substr(str, j, *i - j));
	(*i)++;
	return (s);
}

char	*handle_normal(char *str, int *i, bool heredoc)
{
	int	head;

	head = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '\"'
		&& (str[*i] != '$' || heredoc))
		(*i)++;
	return (ft_substr(str, head, *i - head));
}

char	*expand(char *str, bool heredoc)
{
	char	*expanded;
	int		i;

	expanded = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			expanded = ft_joinfree(expanded, handle_s_quote(str, &i));
		else if (str[i] == '\"')
			expanded = ft_joinfree(expanded, handle_d_quote(str, &i, heredoc));
		else if (str[i] == '$' && !heredoc)
		{
			expanded = expand_dollar(str, expanded, &i);
			if (expanded[0] == '\0' && str[i] == '\0')
			{
				free(expanded);
				return (NULL);
			}
		}
		else
			expanded = ft_joinfree(expanded, handle_normal(str, &i, heredoc));
		if (expanded == NULL)
			err_exit("malloc error");
	}
	return (expanded);
}

char	*exp_dollar(char *str, int *i)
{
	char	*var;
	int		j;

	*(i) += 1;
	if (!ft_isalnum(str[*i]))
		return (ft_strdup("$"));
	j = *i;
	if (ft_isalpha(str[j]) || str[j] == '_')
	{
		while (ft_isalnum(str[j]) || str[j] == '_')
			j++;
	}
	else if (ft_isdigit(str[j]))
		j++;
	var = find_env(&str[*i], j - *i);
	*i = j;
	return (var);
}

char	*expand_documents(char *str)
{
	int		i;
	int		head;
	char	*expanded;

	expanded = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			expanded = ft_joinfree(expanded, exp_dollar(str, &i));
		else
		{
			head = i;
			while (str[i] && str[i] != '$')
				i++;
			expanded = ft_joinfree(expanded, ft_substr(str, head, i - head));
		}
		if (expanded == NULL)
			err_exit("malloc error");
	}
	return (expanded);
}

void	expand_cmd_instance(char **cmd_data, bool here_doc)
{
	char	*tmp;

	if ((ft_strchr(*cmd_data, '\'')
			|| ft_strchr(*cmd_data, '\"')
			|| ft_strchr(*cmd_data, '$'))
		&& !here_doc)
	{
		tmp = *cmd_data;
		*cmd_data = expand(*cmd_data, here_doc);
		free(tmp);
	}
	else
	{
		tmp = *cmd_data;
		*cmd_data = expand_documents(*cmd_data);
		free(tmp);
	}
}

void	expand_cmd_strs(t_cmd *cmd)
{
	int		i;

	i = 0;
	while (cmd->cmd[i] != NULL)
	{
		expand_cmd_instance(&(cmd->cmd[i]), 0);
		if (cmd->cmd[i] == NULL)
			cmd->cmd[i] = ft_xstrdup("");
		i++;
	}
}

void	expand_redir(t_redirect *redirect)
{
	char	*tmp;

	tmp = ft_xstrdup(redirect->file_name);
	expand_cmd_instance(&(redirect->file_name), 0);
	if (redirect->file_name == NULL)
	{
		redirect->file_name = tmp;
		redirect->type = REDIRECT_NONE;
	}
	else
		free(tmp);
}

void	recursive_expansion(t_node *node)
{
	if (node->lhs != NULL)
		expansion(node->lhs);
	if (node->rhs != NULL)
		expansion(node->rhs);
}

void	expand_redir_list(t_node *node)
{
	t_redirect	*head;

	head = node->cmd->redirect_in;
	while (node->cmd->redirect_in->next)
	{
		node->cmd->redirect_in = node->cmd->redirect_in->next;
		if (node->cmd->redirect_in->type == HEREDOC)
		{
			if (node->cmd->redirect_in->delemiter[0] != '\''
				&& node->cmd->redirect_in->delemiter[0] != '\"')
				expand_cmd_instance(&(node->cmd->redirect_in->documents), 1);
		}
		else
			expand_redir(node->cmd->redirect_in);
	}
	node->cmd->redirect_in = head;
	head = node->cmd->redirect_out;
	while (node->cmd->redirect_out->next)
	{
		node->cmd->redirect_out = node->cmd->redirect_out->next;
		expand_redir(node->cmd->redirect_out);
	}
	node->cmd->redirect_out = head;
}

t_node	*expansion(t_node *node)
{
	if (node->lhs == NULL && node->rhs == NULL)
	{
		if (node->cmd->cmd == NULL)
			return (NULL);
		expand_cmd_strs(node->cmd);
		expand_redir_list(node);
	}
	if (node->lhs != NULL || node->rhs != NULL)
		recursive_expansion(node);
	return (node);
}
