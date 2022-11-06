/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta <asouta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:53:04 by shongou           #+#    #+#             */
/*   Updated: 2022/11/05 23:37:16 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	exe_terminal_node(t_node *node, int pipe_flag)
{
	pid_t	pid;

	pid = 0;
	if (pipe_flag == TOP_NODE && which_builtin(node->cmd->cmd))
	{
		g_sh_var.exit_status = exe_process(node->cmd);
	}
	else
	{
		pid = exe_cmd(node->cmd, pipe_flag);
	}
	return (pid);
}

static void	go_through_tree(t_node *node, int pipe_flag)
{
	if (node->lhs != NULL)
		exec(node->lhs, INTERNAL_NODE);
	if (node->rhs != NULL && pipe_flag == 0)
		exec(node->rhs, RIGHT_BOTTOM_NODE);
	else if (node->rhs != NULL)
		exec(node->rhs, INTERNAL_NODE);
}

static void	get_exit_status(pid_t pid)
{
	int		status;
	pid_t	pid2;

	pid2 = 0;
	while (pid2 != -1)
	{
		pid2 = waitpid(-1, &status, 0);
		if (pid == pid2)
		{
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					ft_putendl_fd("Quit: 3", STDERR_FILENO);
				g_sh_var.exit_status = WTERMSIG(status) + 128;
			}
			else if (WIFEXITED(status))
				g_sh_var.exit_status = WEXITSTATUS(status);
		}
	}
}

void	exec(t_node *node, int pipe_flag)
{
	int				backup_stdin;
	int				backup_stdout;
	static pid_t	pid;

	errno = 0;
	if (pipe_flag == TOP_NODE)
	{
		backup_stdin = xdup(STDIN_FILENO);
		backup_stdout = xdup(STDOUT_FILENO);
	}
	if (node->lhs == NULL && node->rhs == NULL)
		pid = exe_terminal_node(node, pipe_flag);
	if (node->lhs != NULL || node->rhs != NULL)
		go_through_tree(node, pipe_flag);
	if (pipe_flag == TOP_NODE)
	{
		xdup2(backup_stdin, STDIN_FILENO);
		xdup2(backup_stdout, STDOUT_FILENO);
		xclose(backup_stdin);
		xclose(backup_stdout);
		get_exit_status(pid);
	}
}
