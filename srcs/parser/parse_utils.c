#include "minishell.h"

void	print_indent(int tab_n)
{
	int	cnt;

	cnt = 0;
	while (cnt++ < tab_n)
		printf(" ");
}

void	print_node(t_node *node, int tab_n)
{
	int	i;
	char	*node_type[2] = {"ND_PIPE", "ND_COMMAND"};

//	printf("ok\n");
	i = 0;
	if (node->lhs == NULL && node->rhs == NULL)
	{
		print_indent(tab_n);
		printf("node_type: %s\n", node_type[node->kind]);
		if (node->cmd->cmd == NULL)
			return ;
		while (node->cmd->cmd[i] != NULL)
		{
			print_indent(tab_n + 2);
			printf("node: %s\n", node->cmd->cmd[i++]);
		}
		while (node->cmd->redirect_in->next)
		{
			node->cmd->redirect_in = node->cmd->redirect_in->next;
			print_indent(tab_n + 4);
			if (node->cmd->redirect_in->type == HEREDOC)
				printf("redirin heredoc: %s\n", node->cmd->redirect_in->delemiter);
			else
				printf("redirin: %s\n", node->cmd->redirect_in->file_name);
		}
		while (node->cmd->redirect_out->next)
		{
			node->cmd->redirect_out = node->cmd->redirect_out->next;
			print_indent(tab_n + 4);
			printf("redirout: %s\n", node->cmd->redirect_out->file_name);
		}
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		print_indent(tab_n);
		printf("node_type: %s\n", node_type[node->kind]);
		if (node->lhs != NULL)
			print_node(node->lhs, tab_n+2);
		if (node->rhs != NULL)
			print_node(node->rhs, tab_n+2);
	}
}
