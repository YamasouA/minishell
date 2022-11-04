/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_others.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:52:51 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:52:53 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_by_cmd_path(t_cmd *cmd, char **envstr)
{
	if (is_directory(cmd->cmd[0]))
	{
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

static void	exec_by_cmd_name(t_cmd *cmd, char **envstr)
{
	char	*path;

	path = check_path(cmd->cmd[0]);
	if (path == NULL || errno != 0)
	{
		if (errno == EACCES)
			print_access_err(path);
		else if (errno == ENOENT)
			print_access_err(cmd->cmd[0]);
		else if (path == NULL)
			print_access_err(cmd->cmd[0]);
	}
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
