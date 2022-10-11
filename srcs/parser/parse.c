#include "minishell.h"

void	free_redirect(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir != NULL)
	{
		tmp = redir;
		redir = redir->next;
		free(tmp->delemiter);
		free(tmp->file_name);
		free(tmp);
	}
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->cmd[i] != NULL)
		free(cmd->cmd[i++]);
	free(cmd->cmd);
	free_redirect(cmd->redirect_in);
	free_redirect(cmd->redirect_out);
}

void	free_token(t_token *tok)
{
	t_token	*tmp;

	while (tok != NULL)
	{
		tmp = tok;
		tok = tok->next;
		free(tmp);
	}
}

void	free_node(t_node *node)
{
	if (node == NULL)
		return ;
	if (node->lhs != NULL)
		free_node(node->lhs);
	if (node->rhs != NULL)
		free_node(node->rhs);
	free_cmd(node->cmd);
	free(node);
}

t_node	*syntax_error(t_node *node, t_token *tok, t_token *head)
{
	size_t	i;

	i = 0;
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if (tok == NULL)
		ft_putstr_fd("newline", STDERR_FILENO);
	else
	{
		while (i < tok->len)
			ft_putchar_fd(tok->str[i++], STDERR_FILENO);
	}
	ft_putendl_fd("'", STDERR_FILENO);
	free_token(head);
	free_node(node);
	return (NULL);
}

t_node	*new_binary(t_node_kind kind, t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node) * 1);
	if (node == NULL || lhs == NULL || rhs == NULL)
	{
		perror("OUT1");
		return (NULL);
	}
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return (node);
}

bool	peek(t_token *tok, char *op)
{
	if (tok->kind != TK_KEYWORD
		|| tok->len != ft_strlen(op)
		|| ft_memcmp(tok->str, op, tok->len))
		return (false);
	return (true);
}

bool	consume(t_token **tok, char *op)
{
	t_token	*t;

	if (!peek(*tok, op))
		return (false);
	t = *tok;
	*tok = (*tok)->next;
	return (true);
}

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)ft_calloc(sizeof(t_node), 1);
	if (node == NULL)
		perror("OUT2");
	node->kind = kind;
	node->cmd = (t_cmd *)ft_calloc(sizeof(t_node), 1);
//	node->cmd->cmd = NULL;
	node->cmd->redirect_in = (t_redirect *)ft_calloc(sizeof(t_cmd), 1);
	node->cmd->redirect_in->next = NULL;
	node->cmd->redirect_out = (t_redirect *)ft_calloc(sizeof(t_cmd), 1);
	node->cmd->redirect_out->next = NULL;
	node->lhs = NULL;
	node->rhs = NULL;
	return (node);
}

size_t	cmd_len(t_token *tok)
{
	size_t	n;

	n = 0;
	while (tok != NULL && !peek(tok, "|"))
	{
		n++;
		tok = tok->next;
	}
	return (n);
}

bool	is_redir(t_token *tok)
{
	return (peek(tok, "<") || peek(tok, ">")
		|| (peek(tok, "<<") || peek(tok, ">>")));
}

int	which_redir(t_token *tok)
{
	if (peek(tok, "<"))
		return (REDIRECT_IN);
	else if (peek(tok, ">"))
		return (REDIRECT_OUT);
	else if (peek(tok, "<<"))
		return (HEREDOC);
	else if (peek(tok, ">>"))
		return (APPEND);
	return (-1);
}

t_redirect	*new_redir(int redir_type, t_token *tok)
{
	t_redirect	*redirect;

	redirect = (t_redirect *)ft_calloc(sizeof(t_cmd), 1);
	redirect->type = redir_type;
	if (redir_type == HEREDOC)
		redirect->delemiter = ft_substr(tok->str, 0, tok->len);
	else
		redirect->file_name = ft_substr(tok->str, 0, tok->len);
	redirect->next = NULL;
	return (redirect);
}

void	add_tail_redir(t_node *node, int redir_type, t_token *tok)
{
	t_redirect	*tmp;
	if (redir_type == HEREDOC || redir_type == REDIRECT_IN)
	{
		tmp = node->cmd->redirect_in;
		while (tmp->next != NULL)
			tmp = tmp->next;
	}
	else
	{
		tmp = node->cmd->redirect_out;
		while (tmp->next != NULL)
			tmp = tmp->next;
	}
	tmp->next = new_redir(redir_type, tok);
}

void	parse_redir(t_token **tok, t_node *node, int type, int *error_flag)
{
	*tok = (*tok)->next;
	if (*tok == NULL || (*tok)->kind == TK_KEYWORD)
	{
		*error_flag = 1;
		//perror("OUT!!");
		return ;
	}
	if (type == REDIRECT_IN || type == HEREDOC)
	{
		if (type == HEREDOC)
			add_tail_redir(node, type, *tok);
		else
			add_tail_redir(node, type, *tok);
	}
	else if (type == REDIRECT_OUT || type == APPEND)
	{
		add_tail_redir(node, type, *tok);
	}
	*tok = (*tok)->next;
}

t_node	*parse_simple_cmd(t_token **tok, t_node *node, int *error_flag, int *heredoc_flag)
{
	int		redir_type;
	size_t	i;

	i = 0;
	while (*tok != NULL && !peek(*tok, "|"))
	{
		redir_type = which_redir(*tok);
		if (redir_type >= 0)
		{
			parse_redir(tok, node, redir_type, error_flag);
			*heredoc_flag = 1;
			if (*error_flag == 1)
				return (node);
		}
		else
		{
			node->cmd->cmd[i] = ft_substr((*tok)->str, 0, (*tok)->len);
			if (node->cmd->cmd[i++] == NULL)
			{
				perror("OUT4!!");
				return (NULL);
			}
			*tok = (*tok)->next;
		}
	}
	node->cmd->cmd[i] = NULL;
	return (node);
}

t_node	*parse_cmd(t_token **tok, int *error_flag, int *heredoc_flag)
{
	t_node	*node;
	t_token	*t;

	if (*tok == NULL)
		return (NULL);
	t = *tok;
	node = new_node(ND_COMMAND);
	if (node == NULL)
	{
		perror("OUT3!!");
		return (NULL);
	}
	node->cmd->cmd = (char **)ft_calloc(sizeof(char *), (cmd_len(*tok) + 1));
	if (node->cmd->cmd == NULL)
	{
		perror("OUT4!!");
		return (NULL);
	}
	if (!parse_simple_cmd(tok, node, error_flag, heredoc_flag))
		return (NULL);
	return (node);
}

char	*read_heredoc(char *deli)
{
	char	*documents;
	char 	*line;

	documents = "";
	while (1)
	{
		line = readline("> ");
		if (line == NULL || strlen(line) == 0)
			documents = ft_strjoin(documents, "\n");
		if (ft_strncmp(line, deli, ft_strlen(deli) + 1) == 0)
		{
			break;
		}
		documents = ft_strjoin(documents, line);
		documents = ft_strjoin(documents, "\n");
	}
	return (documents);
}

void	heredoc(t_node *node)
{
	int	i;
	t_redirect	*tmp;
	
	i = 0;
	if (node->lhs == NULL && node->rhs == NULL)
	{
		tmp = node->cmd->redirect_in;
		while (node->cmd->redirect_in->next)
		{
			node->cmd->redirect_in = node->cmd->redirect_in->next;
			if (node->cmd->redirect_in->type == HEREDOC)
				node->cmd->redirect_in->documents = read_heredoc(node->cmd->redirect_in->delemiter);
		}
		node->cmd->redirect_in = tmp;
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			heredoc(node->lhs);
		if (node->rhs != NULL)
			heredoc(node->rhs);
	}
}

t_node	*parse(t_token *tok)
{
	t_node	*node;
	int		error_flag;
	t_token	*tok_head;
	int	heredoc_flag;

	error_flag = 0;
	heredoc_flag = 0;
	tok_head = tok;
	node = parse_cmd(&tok, &error_flag, &heredoc_flag);
	if (error_flag == 1)
		return (syntax_error(node, tok, tok_head));
	if (node == NULL)
		return (NULL);
	while (tok != NULL && consume(&tok, "|"))
	{
		if (tok == NULL || peek(tok, "|"))
			return (syntax_error(node, tok, tok_head));
		node = new_binary(ND_PIPE, node, parse_cmd(&tok, &error_flag, &heredoc_flag));
		if (error_flag == 1)
			return (syntax_error(node, tok, tok_head));
		/*
		if (node->rhs == NULL)
			return (syntax_error("pipe should have rhs", node, tok));
		*/
		if (node == NULL)
			return (NULL);
	}
	if (heredoc_flag == 1)
		heredoc(node);
	printf("==PARSE==\n");
	print_node(node, 0);
	return (node);
}
