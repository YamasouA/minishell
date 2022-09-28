#include "minishell.h"

void	print_node(t_node *node, int tab_n);

void	syntax_error()
{
	perror("OUT!!");
//	clear_data();
//	init();
//	set_exit_status();
}

//char	**get_cmd()
//{
//	
//}

t_node	*new_binary(t_node_kind kind, t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node) * 1);
	if (node == NULL)
		perror("OUT");
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
	t_token *t;

	if (!peek(*tok, op))
		return false;
	t = *tok;
	*tok = (*tok)->next;
	return true;
}

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node) * 1);
	if (node == NULL)
		perror("OUT");
	node->kind = kind;
	node->cmd = (t_cmd *)malloc(sizeof(t_node) * 1);
	node->cmd->cmd = NULL;
	node->cmd->redirect_in = (t_redirect *)malloc(sizeof(t_cmd) * 1);
	node->cmd->redirect_in->file_name = NULL;
	node->cmd->redirect_out = (t_redirect *)malloc(sizeof(t_cmd) * 1);
	node->cmd->redirect_out->file_name = NULL;
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
		return REDIRECT_IN;
	else if (peek(tok, ">"))
		return REDIRECT_OUT;
	else if (peek(tok, "<<"))
		return HEREDOC;
	else if (peek(tok, ">>"))
		return APPEND;
	return (-1);
		
}

void	parse_redir(t_token **tok, t_node *node, int redir_type)
{
		*tok = (*tok)->next; // include which_redir?
		if (*tok == NULL || (*tok)->kind == TK_KEYWORD)
		{
			perror("OUT!!");
			return ;
		}
		if (redir_type == REDIRECT_IN || redir_type == HEREDOC)
		{
			if (redir_type == HEREDOC)
				node->cmd->redirect_in->delemiter = ft_substr((*tok)->str, 0, (*tok)->len);
			else
				node->cmd->redirect_in->file_name = ft_substr((*tok)->str, 0, (*tok)->len);
			node->cmd->redirect_in->type = redir_type;
		}
		else if (redir_type == REDIRECT_OUT || redir_type == APPEND)
		{
			node->cmd->redirect_out->file_name = ft_substr((*tok)->str, 0, (*tok)->len);
			node->cmd->redirect_out->type = redir_type;
		}	
}

t_node	*cmd(t_token **tok)
{
	t_node	*node;
	t_token	*t;
	size_t	i;
	int		redir_type;

	t = *tok;
	node = new_node(ND_COMMAND);
	if (*tok == NULL)
		return node;
	if (peek(*tok, "|"))
		syntax_error();
	if (node == NULL)
		perror("OUT!!");
	node->cmd->cmd = (char **)malloc(sizeof(char *) * (cmd_len(*tok) + 1));
	if (node->cmd->cmd == NULL)
		perror("OUT!!");
	redir_type = which_redir(*tok);
	if (redir_type >= 0) //cut func parse_redir
	{
		parse_redir(tok, node, redir_type);
	}
	else
	{
		node->cmd->cmd[0] = ft_substr((*tok)->str, 0, (*tok)->len);
	}
	*tok = (*tok)->next;
	i = 1;
	while (*tok != NULL && !peek(*tok, "|"))
	{
		redir_type = which_redir(*tok);
		if (redir_type > 0)
			parse_redir(tok, node, redir_type);
		else
			node->cmd->cmd[i++] = ft_substr((*tok)->str, 0, (*tok)->len);
//		if (*tok == NULL)
//			printf("abc\n");
		*tok = (*tok)->next;
	}
	node->cmd->cmd[i] = NULL;
	return (node);
}

//t_node	*program(t_token *tok)
t_node	*parse(t_token *tok)
{
	t_node	*node;

	node = cmd(&tok);
	while (tok != NULL && consume(&tok, "|"))
	{
	//	printf("aft: %s\n", tok->str);
		if (tok == NULL || peek(tok, "|"))
			syntax_error();
		node = new_binary(ND_PIPE, node, cmd(&tok));
		if (node->rhs == NULL)
			syntax_error();
	}
	print_node(node, 0);
	return (node);
}

void	print_node(t_node *node, int tab_n)
{
	int	i;
	int	cnt;
	char	*node_type[2] = {"ND_PIPE", "ND_COMMAND"};

	cnt = 0;
	while (cnt++ < tab_n)
		printf(" ");
	i = 0;
	if (node->lhs == NULL && node->rhs == NULL)
	{
		if (node->cmd->cmd == NULL)
			return ;
		if (node->cmd->redirect_in->file_name)
			printf("redirin_file_name %s\n", node->cmd->redirect_in->file_name);
		if (node->cmd->redirect_out->file_name)
			printf("redirout_file_name %s\n", node->cmd->redirect_out->file_name);
		while (node->cmd->cmd[i] != NULL)
			printf("node: %s\n", node->cmd->cmd[i++]);
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		printf("node_type: %s\n", node_type[node->kind]);
		if (node->lhs != NULL)
			print_node(node->lhs, tab_n+2);
		if (node->rhs != NULL)
			print_node(node->rhs, tab_n+2);
	}
}

//void parse(t_token list)
//{
//	program();
//}
