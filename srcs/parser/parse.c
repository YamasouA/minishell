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

t_node	*syntax_error(char *msg, t_node *node, t_token *tok)
{
	perror("syntax error: ");
	perror(msg);
	free_token(tok);
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
		perror("OUT2");
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
		*tok = (*tok)->next;
		if (*tok == NULL || (*tok)->kind == TK_KEYWORD)
		{
			//perror("OUT!!");
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
		*tok = (*tok)->next;
}

t_node	*cmd(t_token **tok)
{
	t_node	*node;
	t_token	*t;
	size_t	i;
	int		redir_type;

	if (*tok == NULL)
		//return node;
		return (NULL);

	/* parseの中で確認してるからいらない
	if (peek(*tok, "|"))
		//return (syntax_error(node));
		return (NULL);
	*/
	t = *tok;
	node = new_node(ND_COMMAND);
	if (node == NULL)
	{
		perror("OUT3!!");
		return (NULL);
	}
	node->cmd->cmd = (char **)malloc(sizeof(char *) * (cmd_len(*tok) + 1));
	if (node->cmd->cmd == NULL)
	{
		perror("OUT4!!");
		return (NULL);
	}
	i = 0;
	while (*tok != NULL && !peek(*tok, "|"))
	{
		redir_type = which_redir(*tok);
		if (redir_type >= 0)
		{
			parse_redir(tok, node, redir_type);
		}
		else
		{
			node->cmd->cmd[i++] = ft_substr((*tok)->str, 0, (*tok)->len);
			*tok = (*tok)->next;
		}
	}
	node->cmd->cmd[i] = NULL;
	return (node);
}

//t_node	*program(t_token *tok)
t_node	*parse(t_token *tok)
{
	t_node	*node;

	node = cmd(&tok);
	if (node == NULL)
		return (NULL);
	while (tok != NULL && consume(&tok, "|"))
	{
		if (tok == NULL)
			return (syntax_error("pipe should have rhs", node, tok));
		if (peek(tok, "|"))
			return (syntax_error("consecutive pipe", node, tok));
		node = new_binary(ND_PIPE, node, cmd(&tok));
		if (node->rhs == NULL)
			return (syntax_error("pipe should have rhs", node, tok));
		if (node == NULL || node->rhs == NULL)
			return (NULL);
	}
	print_node(node, 0);
	return (node);
}
