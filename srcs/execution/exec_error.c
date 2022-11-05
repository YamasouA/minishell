/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta <asouta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:52:46 by shongou           #+#    #+#             */
/*   Updated: 2022/11/05 23:36:55 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exec_process_error(char *cmd, char *msg, int status)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
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
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": is a directory\n", STDERR_FILENO);
}

int	print_redirect_err(char *file_name)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(file_name, STDERR_FILENO);
	if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else if (errno == ENOENT)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
	return (0);
}
