#include "minishell.h"

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
