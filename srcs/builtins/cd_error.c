/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:51:03 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:51:06 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*no_current_dir(char *path)
{
	char	*pwd;
	char	*newpwd;

	ft_putendl_fd("cd: error retrieving current directory: \
		getcwd: cannot access parent directories: \
		No such file or directory", STDERR_FILENO);
	pwd = search_key(g_sh_var.environ, "PWD");
	newpwd = join_with_connector(pwd, path, '/');
	return (newpwd);
}

void	print_error(char *dir, char *msg)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(dir, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}
