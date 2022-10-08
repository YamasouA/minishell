#include "minishell.h"

void	exe_cmd(t_cmd *cmd)
{
	int	fd[2];
	pid_t	pid;

	if (pipe(fd) < 0)
		perror("OUT!");
	pid = fork();
	if (pid < 0)
		perror("OUT!");
	if (pid == 0)
	{
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		exe_process(cmd);
	}
	else
	{
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		return ;
	}


}

void	exec(t_node *node)
{
	if (node->lhs == NULL && node->rhs == NULL)
	{
		if (node->cmd->cmd == NULL)
			return ;
		if (node->cmd->cmd[0] != NULL)
		{
			exe_cmd(node->cmd);
		}
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			exec(node->lhs);
		if (node->rhs != NULL)
			exec(node->rhs);
	}
	waitpid(-1, &WEXITSTATUS, 0);
}
