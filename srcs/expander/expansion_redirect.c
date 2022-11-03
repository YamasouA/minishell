#include "minishell.h"

void	expand_redir(t_redirect *redirect)
{
	char	*tmp;

	tmp = ft_xstrdup(redirect->file_name);
	expand_cmd_instance(&(redirect->file_name), 0);
	if (redirect->file_name == NULL)
	{
		redirect->file_name = tmp;
		redirect->type = REDIRECT_NONE;
	}
	else
		free(tmp);
}

void	expand_redir_list(t_node *node)
{
	t_redirect	*head;

	head = node->cmd->redirect_in;
	while (node->cmd->redirect_in->next)
	{
		node->cmd->redirect_in = node->cmd->redirect_in->next;
		if (node->cmd->redirect_in->type == HEREDOC)
		{
			if (node->cmd->redirect_in->delemiter[0] != '\''
				&& node->cmd->redirect_in->delemiter[0] != '\"')
				expand_cmd_instance(&(node->cmd->redirect_in->documents), 1);
		}
		else
			expand_redir(node->cmd->redirect_in);
	}
	node->cmd->redirect_in = head;
	head = node->cmd->redirect_out;
	while (node->cmd->redirect_out->next)
	{
		node->cmd->redirect_out = node->cmd->redirect_out->next;
		expand_redir(node->cmd->redirect_out);
	}
	node->cmd->redirect_out = head;
}

