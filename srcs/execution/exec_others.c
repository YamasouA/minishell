#include "minishell.h"

void	exec_by_cmd_path(t_cmd *cmd, char **envstr)
{
	if (is_directory(cmd->cmd[0]))
	{
		//ft_putstr_fd("minishell: ", 2);
		//ft_putstr_fd(cmd->cmd[0], 2);
		//ft_putstr_fd(": is a directory\n", 2);
		err_is_a_directory(cmd->cmd[0]);
		exit(126);
	}
	else if (!access(cmd->cmd[0], X_OK))
	{
		execve(cmd->cmd[0], cmd->cmd, envstr);
		err_exit("execve error: ");
	}
	else
		print_access_err(cmd->cmd[0]);
}

void	exec_by_cmd_name(t_cmd *cmd, char **envstr)
{
	char	*path;

	path = check_path(cmd->cmd[0]);
	if (path == NULL || errno != 0)
	{
		if (errno == EACCES)
			print_access_err(path);
		else if (errno == ENOENT)
			print_access_err(cmd->cmd[0]);
		else if (path == NULL) //&& errno == 0)
			print_access_err(cmd->cmd[0]);
//			perror("OUT1");
	}
//		exit(execve(path, cmd->cmd, envstr));
	execve(path, cmd->cmd, envstr);
	err_exit("execve error: ");
}

void	exec_others(t_cmd *cmd)
{
	char	**envstr;

	envstr = envlist_to_str(g_sh_var.environ);
	if (is_path(cmd->cmd[0]))
	{
		exec_by_cmd_path(cmd, envstr);
	}
	else
	{
		exec_by_cmd_name(cmd, envstr);
	}
}
