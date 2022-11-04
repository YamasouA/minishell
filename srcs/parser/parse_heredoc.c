/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:54:37 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:54:38 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_loop(char *exp_deli, bool *heredoc_err)
{
	char	*line;
	char	*documents;

	documents = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (g_sh_var.signal != 0)
		{
			processing_on_signal(line, documents, heredoc_err);
			return (NULL);
		}
		if (line == NULL)
			break ;
		if (ft_strncmp(line, exp_deli, ft_strlen(exp_deli) + 1) == 0)
		{
			free(line);
			break ;
		}
		documents = ft_joinfree(documents, line);
		documents = ft_joinfree(documents, ft_strdup("\n"));
		if (documents == NULL)
			err_exit("malloc error: ");
	}
	return (documents);
}

static char	*read_heredoc(char *deli, bool *heredoc_err)
{
	char	*documents;
	char	*exp_deli;

	exp_deli = expand(deli, 1);
	rl_event_hook = check_state;
	set_signal_handler(SIGINT, heredoc_signal_handler);
	documents = heredoc_loop(exp_deli, heredoc_err);
	set_signal_handler(SIGINT, SIG_IGN);
	free(exp_deli);
	return (documents);
}

static void	get_documents(t_redirect *redirect_in, bool *err)
{
	t_redirect	*tmp;
	char		*numstr;

	tmp = redirect_in;
	while (redirect_in->next)
	{
		redirect_in = redirect_in->next;
		if (redirect_in->type == HEREDOC)
		{
			redirect_in->documents = read_heredoc(redirect_in->delimiter, err);
			numstr = ft_ultoa(xorshift());
			redirect_in->file_name = ft_strjoin(TMPFILE, numstr);
			if (redirect_in->file_name == NULL)
				err_exit("malloc error: ");
			free(numstr);
		}
	}
	redirect_in = tmp;
}

static void	heredoc(t_node *node, bool *heredoc_err)
{
	if (node->lhs == NULL && node->rhs == NULL)
	{
		get_documents(node->cmd->redirect_in, heredoc_err);
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			heredoc(node->lhs, heredoc_err);
		if (node->rhs != NULL)
			heredoc(node->rhs, heredoc_err);
	}
}

void	do_heredoc(t_node *node, bool *heredoc_err, int heredoc_flag)
{
	if (1 <= heredoc_flag && heredoc_flag <= 16)
		heredoc(node, heredoc_err);
	else if (heredoc_flag > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		exit(2);
	}
}
