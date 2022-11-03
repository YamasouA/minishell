#include "minishell.h"

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
//void	print_indent(int tab_n)
//{
//	int	cnt;
//
//	cnt = 0;
//	while (cnt++ < tab_n)
//		printf(" ");
//}
//
//void	print_node(t_node *node, int tab_n)
//{
//	int	i;
//	char	*node_type[2] = {"ND_PIPE", "ND_COMMAND"};
//	t_redirect	*tmp;
//
//	i = 0;
//	if (node->lhs == NULL && node->rhs == NULL)
//	{
//		print_indent(tab_n);
//		printf("node_type: %s\n", node_type[node->kind]);
//		if (node->cmd->cmd == NULL)
//			return ;
//		while (node->cmd->cmd[i] != NULL)
//		{
//			print_indent(tab_n + 2);
//			printf("node: %s\n", node->cmd->cmd[i++]);
//		}
//		//while (node->cmd->redirect_in->next)
//		tmp = node->cmd->redirect_in;
//		while (node->cmd->redirect_in->next)
//		{
//			node->cmd->redirect_in = node->cmd->redirect_in->next;
//			print_indent(tab_n + 4);
//			if (node->cmd->redirect_in->type == HEREDOC)
//			{
//				printf("redirin heredoc: %s\n", node->cmd->redirect_in->delemiter);
//				if (node->cmd->redirect_in->documents)
//					printf("documets: %s\n", node->cmd->redirect_in->documents);
//			}
//			else
//				printf("redirin: %s\n", node->cmd->redirect_in->file_name);
//		}
//		node->cmd->redirect_in = tmp;
//		tmp = node->cmd->redirect_out;
//		while (node->cmd->redirect_out->next)
//		{
//			node->cmd->redirect_out = node->cmd->redirect_out->next;
//			print_indent(tab_n + 4);
//			printf("redirout: %s\n", node->cmd->redirect_out->file_name);
//		}
//		node->cmd->redirect_out = tmp;
//	}
//	if (node->lhs != NULL || node->rhs != NULL)
//	{
//		print_indent(tab_n);
//		printf("node_type: %s\n", node_type[node->kind]);
//		if (node->lhs != NULL)
//			print_node(node->lhs, tab_n+2);
//		if (node->rhs != NULL)
//			print_node(node->rhs, tab_n+2);
//	}
//}
