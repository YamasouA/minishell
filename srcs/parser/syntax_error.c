#include "minishell.h"

//free or syntax_error
void	free_redirect(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir != NULL)
	{
		tmp = redir;
		redir = redir->next;
		free(tmp->delimiter);
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

void	free_token(t_token *tok) //same free_token_list ?
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
//so far or

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
//so far
