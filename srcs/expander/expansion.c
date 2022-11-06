/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:53:53 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:53:55 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand(char *str, bool heredoc)
{
	char	*expanded;
	int		i;

	expanded = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			expanded = ft_joinfree(expanded, handle_s_quote(str, &i));
		else if (str[i] == '\"')
			expanded = ft_joinfree(expanded, handle_d_quote(str, &i, heredoc));
		else if (str[i] == '$' && !heredoc)
		{
			expanded = expand_dollar(str, expanded, &i);
			if (is_expand_none(str, expanded, i))
				return (NULL);
		}
		else
			expanded = ft_joinfree(expanded, handle_normal(str, &i, heredoc));
		if (expanded == NULL)
			err_exit("malloc error");
	}
	return (expanded);
}

void	expand_cmd_instance(char **cmd_data, bool here_doc)
{
	char	*tmp;

	if ((ft_strchr(*cmd_data, '\'')
			|| ft_strchr(*cmd_data, '\"')
			|| ft_strchr(*cmd_data, '$'))
		&& !here_doc)
	{
		tmp = *cmd_data;
		*cmd_data = expand(*cmd_data, here_doc);
		free(tmp);
	}
	else if (here_doc)
	{
		tmp = *cmd_data;
		*cmd_data = expand_documents(*cmd_data);
		free(tmp);
	}
}

static void	expand_cmd_strs(t_cmd *cmd)
{
	int		i;

	i = 0;
	while (cmd->cmd[i] != NULL)
	{
		expand_cmd_instance(&(cmd->cmd[i]), 0);
		if (cmd->cmd[i] == NULL)
			cmd->cmd[i] = ft_xstrdup("");
		i++;
	}
}

static void	recursive_expansion(t_node *node)
{
	if (node->lhs != NULL)
		expansion(node->lhs);
	if (node->rhs != NULL)
		expansion(node->rhs);
}

t_node	*expansion(t_node *node)
{
	if (node->lhs == NULL && node->rhs == NULL)
	{
		expand_cmd_strs(node->cmd);
		expand_redir_list(node);
	}
	if (node->lhs != NULL || node->rhs != NULL)
		recursive_expansion(node);
	return (node);
}
