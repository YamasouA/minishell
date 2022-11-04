#include "minishell.h"
								//t_cmd *cmd is better?
static pid_t	exe_terminal_node(t_node *node, int pipe_flag)
{
	pid_t	pid;

	pid = 0;
	if (!pipe_flag && which_builtin(node->cmd->cmd))
	{
		g_sh_var.exit_status = exe_process(node->cmd);
	}
	else
//	else if (node->cmd->cmd[0] != NULL)
	{
		pid = exe_cmd(node->cmd, pipe_flag);
	}
	return (pid);
}

static void	go_through_tree(t_node *node, int pipe_flag)
{
	if (node->lhs != NULL)
		exec(node->lhs, 1);
	if (node->rhs != NULL && pipe_flag == 0)
		exec(node->rhs, 2);
	else if (node->rhs != NULL)
		exec(node->rhs, 1);
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
					ft_putendl_fd("Quit: 3", 2);
				//else if(WTERMSIG(status) == SIGINT)
					//ft_putendl_fd("", 2);
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
	//g_sh_var.exit_status = 0;
	if (pipe_flag == 0)
	{
		backup_stdin = xdup(0);
		backup_stdout = xdup(1);
	}
	if (node->lhs == NULL && node->rhs == NULL)
		pid = exe_terminal_node(node, pipe_flag);
	if (node->lhs != NULL || node->rhs != NULL)
		go_through_tree(node, pipe_flag);
	if (pipe_flag == 0)
	{
		xdup2(backup_stdin, 0);
		xdup2(backup_stdout, 1);
		xclose(backup_stdin);
		xclose(backup_stdout);
		get_exit_status(pid);
	}
}
