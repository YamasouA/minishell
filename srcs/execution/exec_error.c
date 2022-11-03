#include "minishell.h"

void	print_exec_process_error(char *cmd, char *msg, int status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	if (status == EACCES)
		exit(126);
	else if (status == ENOENT || status == -1)
		exit(127);
}

void	print_access_err(char *msg)
{
	if (errno == EACCES)
		print_exec_process_error(msg, ": Permission denied", EACCES);
	else if (errno == ENOENT && !is_path(msg))
		print_exec_process_error(msg, ": command not found", ENOENT);
	else
		print_exec_process_error(msg, ": No such file or directory", -1);
}

void	err_is_a_directory(char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": is a directory\n", 2);
}

int	print_redirect_err(char *file_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file_name, 2);
	if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (errno == ENOENT)
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(": ambiguous redirect\n", 2);
	return (0);
//	return (1);
}
//
