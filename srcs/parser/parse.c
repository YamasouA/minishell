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
	node->cmd = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
//	node->cmd->cmd = NULL;
	node->cmd->redirect_in = (t_redirect *)ft_calloc(sizeof(t_redirect), 1);
	node->cmd->redirect_in->next = NULL;
	node->cmd->redirect_out = (t_redirect *)ft_calloc(sizeof(t_redirect), 1);
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
			if (redir_type == HEREDOC)
				*heredoc_flag += 1;
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

#define TMPFILE "tmp/tempfile"

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
		return (NULL);
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
	g_signal = sig;
}

static int	check_state() {
	if (g_signal != 0) {
		//g_signal = 0;
		rl_done = 1;
	}
	return 0;
}

char	*read_heredoc(char *deli, bool *heredoc_err)
{
	char	*documents;
	char 	*line;
	char	*exp_deli;
	//pid_t	pid;
	//int		status;
//	int		doc_len;

	//signal(SIGINT, SIG_IGN);
//	signal(SIGINT, heredoc_signal_handler);
	exp_deli = expand(deli, 1);
	documents = ft_strdup("");
	rl_event_hook = check_state;
	signal(SIGINT, heredoc_signal_handler);
	while (1)
	{
		line = readline("> ");
		if (g_signal != 0)
		{
			*heredoc_err = 1;
			//printf("\n");
			//rl_on_new_line();
			//rl_replace_line("", 0);
			//rl_redisplay();
			signal(SIGINT, SIG_IGN);
			return (NULL);
		}
		//printf("line:%s;", line);
		if (line == NULL)// || strlen(line) == 0)
		{
//			printf("\e[1A\e[2C");
			break ;
		}
		if (ft_strncmp(line, exp_deli, ft_strlen(exp_deli) + 1) == 0)
		{
			//documents = check_quote(documents, deli);
			break;
		}
		documents = ft_strjoin(documents, line);
		documents = ft_strjoin(documents, "\n");
	}
//	doc_len = ft_strlen(documents);
//	if (doc_len)
//		documents[doc_len - 1] = '\0';
//	printf("%s", documents);
	signal(SIGINT, SIG_IGN);
	return (documents);
}

void	heredoc(t_node *node, bool *heredoc_err)
{
	int	i;
	char	*numstr;
	t_redirect	*tmp;
	
	i = 0;
	if (node->lhs == NULL && node->rhs == NULL)
	{
		tmp = node->cmd->redirect_in;
		while (node->cmd->redirect_in->next)
		{
			node->cmd->redirect_in = node->cmd->redirect_in->next;
			if (node->cmd->redirect_in->type == HEREDOC)
			{
				node->cmd->redirect_in->documents = read_heredoc(node->cmd->redirect_in->delemiter, heredoc_err);
				//printf("heredoc: %d\n", heredoc_err);
				numstr = ft_ultoa(xorshift());
//				printf("%s\n", numstr);
				node->cmd->redirect_in->file_name = ft_strjoin(TMPFILE, numstr);
//				printf("%s\n", node->cmd->redirect_in->file_name);
				free(numstr);
			}
		}
		node->cmd->redirect_in = tmp;
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			heredoc(node->lhs, heredoc_err);
		if (node->rhs != NULL)
			heredoc(node->rhs, heredoc_err);
	}
}

t_node	*parse(t_token *tok, bool *heredoc_err)
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
	if (1 <= heredoc_flag && heredoc_flag <= 16)
		heredoc(node, heredoc_err);
	else if (heredoc_flag > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		exit(2);
	}
//	printf("==PARSE==\n");
//	print_node(node, 0);
	return (node);
}
