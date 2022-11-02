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
		free(tmp->documents);
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
	if (node->rhs == NULL && node->lhs == NULL)
	{
		free_cmd(node->cmd);
		free(node->cmd);
		node->cmd = NULL;
	}
	free(node);
}

t_node	*syntax_error(t_node *node, t_token **tok, t_token *head)
{
	size_t	i;

	i = 0;
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if (*tok == NULL)
		ft_putstr_fd("newline", STDERR_FILENO);
	else
	{
		while (i < (*tok)->len)
			ft_putchar_fd((*tok)->str[i++], STDERR_FILENO);
	}
	ft_putendl_fd("'", STDERR_FILENO);
	free_token(head);
	*tok = NULL;
	free_node(node);
	node = NULL;
	g_sh_var.exit_status = 258;
	return (NULL);
}

t_node	*new_binary(t_node_kind kind, t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node) * 1);
	if (node == NULL)
		err_exit("malloc error: ");
	//if (node == NULL || lhs == NULL || rhs == NULL)
//	if (lhs == NULL || rhs == NULL)
//	{
//		return (NULL);
//	}
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
	if (!peek(*tok, op))
		return (false);
	*tok = (*tok)->next;
	return (true);
}

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)ft_calloc(sizeof(t_node), 1);
	if (node == NULL)
		err_exit("malloc error: ");
	node->kind = kind;
	node->cmd = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
	if (node->cmd == NULL)
		err_exit("malloc error: ");
	//node->cmd->cmd = NULL;
	node->cmd->redirect_in = (t_redirect *)ft_calloc(sizeof(t_redirect), 1);
	if (node->cmd->redirect_in == NULL)
		err_exit("malloc error: ");
	node->cmd->redirect_in->next = NULL;
	node->cmd->redirect_out = (t_redirect *)ft_calloc(sizeof(t_redirect), 1);
	if (node->cmd->redirect_out == NULL)
		err_exit("malloc error: ");
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

	redirect = (t_redirect *)ft_calloc(sizeof(t_redirect), 1);
	if (redirect == NULL)
		err_exit("malloc error: ");
	redirect->type = redir_type;
	if (redir_type == HEREDOC)
	{
		redirect->delemiter = ft_substr(tok->str, 0, tok->len);
		if (redirect->delemiter == NULL)
			err_exit("malloc error: ");
	}
	else
	{
		redirect->file_name = ft_substr(tok->str, 0, tok->len);
		if (redirect->file_name == NULL)
			err_exit("malloc error: ");
	}
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

t_node	*parse_simple_cmd(t_token **tok, t_node *node, int *err, int *heredoc)
{
	int		redir_type;
	size_t	i;

	i = 0;
	while (*tok != NULL && !peek(*tok, "|"))
	{
		redir_type = which_redir(*tok);
		if (redir_type >= 0)
		{
			parse_redir(tok, node, redir_type, err);
			if (redir_type == HEREDOC)
				*heredoc += 1;
			if (*err == 1)
				return (node);
		}
		else
		{
			node->cmd->cmd[i] = ft_substr((*tok)->str, 0, (*tok)->len);
			if (node->cmd->cmd[i++] == NULL)
				err_exit("malloc error: ");
			*tok = (*tok)->next;
		}
	}
	node->cmd->cmd[i] = NULL;
	return (node);
}

t_node	*parse_cmd(t_token **tok, int *error_flag, int *heredoc_flag)
{
	t_node	*node;

	if (*tok == NULL)
		return (NULL);
	node = new_node(ND_COMMAND);
	node->cmd->cmd = (char **)ft_calloc(sizeof(char *), (cmd_len(*tok) + 1));
	if (node->cmd->cmd == NULL)
	{
		err_exit("malloc error: ");
	}
	if (peek(*tok, "|") == 1)
	{
		*error_flag = 1;
		return (node);
	}
	parse_simple_cmd(tok, node, error_flag, heredoc_flag);
	return (node);
}

static int	count_digit(unsigned long n)
{
	int				count;
	unsigned long	num;

	count = 1;
	num = n;
	while (num > 9)
	{
		num = num / 10;
		count++;
	}
	return (count);
}

char	*ft_ultoa(unsigned long n)
{
	int				figure_len;
	char			*numstr;
	unsigned long	num;

	figure_len = count_digit(n);
	numstr = (char *)malloc(sizeof(char) * (figure_len + 1));
	if (!numstr)
		err_exit("malloc error: ");
	numstr[figure_len] = '\0';
	if (n == 0)
		numstr[0] = '0';
	num = n;
	while (num)
	{
		numstr[--figure_len] = (num % 10) + '0';
		num = num / 10;
	}
	return (numstr);
}

unsigned long	xorshift(void)
{
	static unsigned long	x = 123456789;
	static unsigned long	y = 362436069;
	static unsigned long	z = 521288629;
	static unsigned long	w = 88675123;
	unsigned long			t;

	t = (x ^ (x << 11));
	x = y;
	y = z;
	z = w;
	w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	return (w);
}

static void	heredoc_signal_handler(int sig)
{
	g_sh_var.signal = sig;
}

static int	check_state(void)
{
	if (g_sh_var.signal != 0)
	{
		//g_sh_var.signal = 0;
		rl_done = 1;
	}
	return (0);
}

void	processing_on_signal(char *line, char *documents, bool *heredoc_err)
{
	*heredoc_err = 1;
	set_signal_handler(SIGINT, SIG_IGN);
	free(line);
	free(documents);
}

char	*heredoc_loop(char *exp_deli, bool *heredoc_err)
{
	char	*line;
	char	*documents;

	documents = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (g_sh_var.signal != 0)
		{
			processing_on_signal(line, documents, heredoc_err);
			return (NULL);
		}
		if (line == NULL)
			break ;
		if (ft_strncmp(line, exp_deli, ft_strlen(exp_deli) + 1) == 0)
		{
			free(line);
			break ;
		}
		documents = ft_joinfree(documents, line);
		documents = ft_joinfree(documents, ft_strdup("\n"));
		if (documents == NULL)
			err_exit("malloc error: ");
	}
	return (documents);
}

char	*read_heredoc(char *deli, bool *heredoc_err)
{
	char	*documents;
	char	*exp_deli;

	exp_deli = expand(deli, 1);
	rl_event_hook = check_state;
	set_signal_handler(SIGINT, heredoc_signal_handler);
	documents = heredoc_loop(exp_deli, heredoc_err);
	set_signal_handler(SIGINT, SIG_IGN);
	free(exp_deli);
	return (documents);
}

void	get_documents(t_redirect *redirect_in, bool *err)
{
	t_redirect	*tmp;
	char		*numstr;

	tmp = redirect_in;
	while (redirect_in->next)
	{
		redirect_in = redirect_in->next;
		if (redirect_in->type == HEREDOC)
		{
			redirect_in->documents = read_heredoc(redirect_in->delemiter, err);
			numstr = ft_ultoa(xorshift());
			redirect_in->file_name = ft_strjoin(TMPFILE, numstr);
			if (redirect_in->file_name == NULL)
				err_exit("malloc error: ");
			free(numstr);
		}
	}
	redirect_in = tmp;
}

void	heredoc(t_node *node, bool *heredoc_err)
{
	if (node->lhs == NULL && node->rhs == NULL)
	{
		get_documents(node->cmd->redirect_in, heredoc_err);
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			heredoc(node->lhs, heredoc_err);
		if (node->rhs != NULL)
			heredoc(node->rhs, heredoc_err);
	}
}

void	do_heredoc(t_node *node, bool *heredoc_err, int heredoc_flag)
{
	if (1 <= heredoc_flag && heredoc_flag <= 16)
		heredoc(node, heredoc_err);
	else if (heredoc_flag > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		exit(2);
	}	
}

t_node	*parse(t_token **tok, bool *heredoc_err)
{
	t_node	*node;
	int		error_flag;
	t_token	*tok_head;
	int		heredoc_flag;

	error_flag = 0;
	heredoc_flag = 0;
	tok_head = *tok;
	node = parse_cmd(tok, &error_flag, &heredoc_flag);
	if (error_flag == 1)
		return (syntax_error(node, tok, tok_head));
//	if (node == NULL)
//		return (NULL);
	while (*tok != NULL && consume(tok, "|"))
	{
		if (tok == NULL || peek(*tok, "|"))
			return (syntax_error(node, tok, tok_head));
		node = new_binary(ND_PIPE, node, \
				parse_cmd(tok, &error_flag, &heredoc_flag));
		if (error_flag == 1)
			return (syntax_error(node, tok, tok_head));
	}
	do_heredoc(node, heredoc_err, heredoc_flag);
	*tok = tok_head;
	return (node);
}
