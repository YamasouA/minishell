#include "minishell.h"

void	print_node(t_node *node, int tab_n);

void	syntax_error()
{
	perror("OUT!!");
}

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
	node->lhs = NULL;
	node->rhs = NULL;
	return (node);
}

t_node	*cmd(t_token **tok)
{
	t_node	*node;
	t_token	*t;
//	char	*cmd;
	char	*tmp;
	char	*tmp2;
	char	*tmp3;

	t = *tok;
	node = new_node(ND_COMMAND);
	if (*tok == NULL)
		return node;
	if (peek(*tok, "|"))
		syntax_error();
	if (node == NULL)
		perror("OUT!!");

	node->cmd = ft_substr((*tok)->str, 0, (*tok)->len);
	if (node->cmd == NULL)
		perror("OUT!!");
	*tok = (*tok)->next;
	while (*tok != NULL && !peek(*tok, "|"))
	{
		tmp = node->cmd;
		tmp2 = ft_substr((*tok)->str, 0, (*tok)->len);
		if (node->cmd == NULL)
			perror("OUT!!");
		node->cmd = ft_strjoin(node->cmd, " ");
		if (node->cmd == NULL)
			perror("OUT!!");
		tmp3 = node->cmd;
		node->cmd = ft_strjoin(node->cmd, tmp2);
		if (node->cmd == NULL)
			perror("OUT!!");
		free(tmp);
		free(tmp2);
		free(tmp3);
		*tok = (*tok)->next;
	}
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
	int	cnt;
	char	*node_type[2] = {"ND_PIPE", "ND_COMMAND"};

	cnt = 0;
	while (cnt++ < tab_n)
		printf(" ");
	if (node->lhs == NULL && node->rhs == NULL)
		printf("node: %s\n", node->cmd);
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
