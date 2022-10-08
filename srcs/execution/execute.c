#include "minishell.h"

bool	is_redirect(t_cmd *cmd)
{
	if (cmd->redirect_in->next == NULL && cmd->redirect_out->next == NULL)
		return (false);
	return (true);
}

bool	which_builtin(char **cmd)
{
	static char	*builtins[] = {"cd", "echo", "unset", "export", "exit", "pwd", "env"};
	size_t	i;

	i = 0;
	while (builtins[i] != NULL)
	{
		if (ft_strncmp(builtins[i], cmd[0], ft_strlen(builtins[i]) + 1) == 0)
			return (true);
		i++;
	}
	return (false);
}

void	exec_builtin(t_cmd *cmd)
{
	static char	*builtins[] = {"cd", "echo", "unset", "export", "exit", "pwd", "env"};
	static int (*builtin_func[])(char **) = {&ft_cd, &ft_echo, &ft_unset, &ft_export, &ft_exit, &ft_pwd, &ft_env};
	int			i;

	i = 0;
	while (i < sizeof(builtin_str / sizeof(char *)))
	{
		if (ft_strncmp(cmd->cmd[0], builtin_str[i], ft_strlen(builtin_str[i]) == 0))
			return (*builtin_func[i](cmd->cmd);
	}
}

void	exe_process(t_cmd *cmd)
{
	if (is_redirect(cmd))
		do_redirect(cmd);
	if (is_builtins(cmd->cmd))
		exec_builtin(cmd);
	else
		exec_others(cmd);
}

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

void	exec(t_node *node, int pipe_flag)
{
	if (node->lhs == NULL && node->rhs == NULL)
	{
		if (!pipe_flag && is_builtins(node->cmd->cmd))
		{
			exe_process(node->cmd);
		}
		else if (node->cmd->cmd[0] != NULL)
		{
			exe_cmd(node->cmd);
		}
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			exec(node->lhs, 1);
		if (node->rhs != NULL)
			exec(node->rhs, 1);
	}
	waitpid(-1, &WEXITSTATUS, 0);
}
