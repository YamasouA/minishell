/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:55:24 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:55:26 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*parse_simple_cmd(t_token **tok, t_node *node, \
		int *err, int *heredoc)
{
	int		redir_type;
	size_t	i;

	i = 0;
	while (*tok != NULL && !peek(*tok, "|"))
	{
		redir_type = which_redir(*tok);
		if (redir_type >= 0)
		{
			parse_redir(tok, node, redir_type, err);
			if (redir_type == HEREDOC)
				*heredoc += 1;
			if (*err == 1)
				return (node);
		}
		else
		{
			node->cmd->cmd[i] = ft_substr((*tok)->str, 0, (*tok)->len);
			if (node->cmd->cmd[i++] == NULL)
				err_exit("malloc error: ");
			*tok = (*tok)->next;
		}
	}
	node->cmd->cmd[i] = NULL;
	return (node);
}

static t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)ft_calloc(sizeof(t_node), 1);
	if (node == NULL)
		err_exit("malloc error: ");
	node->kind = kind;
	node->cmd = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
	if (node->cmd == NULL)
		err_exit("malloc error: ");
	node->cmd->redirect_in = (t_redirect *)ft_calloc(sizeof(t_redirect), 1);
	if (node->cmd->redirect_in == NULL)
		err_exit("malloc error: ");
	node->cmd->redirect_in->next = NULL;
	node->cmd->redirect_out = (t_redirect *)ft_calloc(sizeof(t_redirect), 1);
	if (node->cmd->redirect_out == NULL)
		err_exit("malloc error: ");
	node->cmd->redirect_out->next = NULL;
	node->lhs = NULL;
	node->rhs = NULL;
	return (node);
}

static t_node	*parse_cmd(t_token **tok, int *error_flag, int *heredoc_flag)
{
	t_node	*node;

	if (*tok == NULL)
		return (NULL);
	node = new_node(ND_COMMAND);
	node->cmd->cmd = (char **)ft_calloc(sizeof(char *), (cmd_len(*tok) + 1));
	if (node->cmd->cmd == NULL)
	{
		err_exit("malloc error: ");
	}
	if (peek(*tok, "|") == 1)
	{
		*error_flag = 1;
		return (node);
	}
	parse_simple_cmd(tok, node, error_flag, heredoc_flag);
	return (node);
}

static t_node	*new_binary(t_node_kind kind, t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node) * 1);
	if (node == NULL)
		err_exit("malloc error: ");
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return (node);
}

t_node	*parse(t_token **tok)
{
	t_node	*node;
	int		error_flag;
	t_token	*tok_head;
	int		heredoc_flag;

	error_flag = 0;
	heredoc_flag = 0;
	tok_head = *tok;
	node = parse_cmd(tok, &error_flag, &heredoc_flag);
	if (error_flag == 1)
		return (syntax_error(node, tok, tok_head));
	while (*tok != NULL && consume(tok, "|"))
	{
		if (tok == NULL || peek(*tok, "|"))
			return (syntax_error(node, tok, tok_head));
		node = new_binary(ND_PIPE, node, \
				parse_cmd(tok, &error_flag, &heredoc_flag));
		if (error_flag == 1)
			return (syntax_error(node, tok, tok_head));
	}
	do_heredoc(node, heredoc_flag);
	*tok = tok_head;
	return (node);
}
