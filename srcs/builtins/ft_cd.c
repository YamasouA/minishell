/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:51:24 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:51:26 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_pwd(char *path)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		pwd = no_current_dir(path);
	update_or_add_value(&g_sh_var.environ, "OLDPWD", \
		search_key(g_sh_var.environ, "PWD"));
	update_or_add_value(&g_sh_var.environ, "PWD", pwd);
	free(pwd);
}

static int	get_path(char *s, char **path)
{
	int	status;

	status = -1;
	if (*s == '\0')
		*path = ft_xstrdup(search_key(g_sh_var.environ, "HOME"));
	else if (ft_strncmp(s, "-", 2) == 0)
		*path = ft_xstrdup(search_key(g_sh_var.environ, "OLDPWD"));
	else
		*path = ft_xstrdup(s);
	if (*path != NULL)
		status = 0;
	if (*path == NULL && *s == '\0')
		*path = ft_xstrdup("HOME");
	if (*path == NULL && ft_strncmp(s, "-", 2) == 0)
		*path = ft_xstrdup("OLDPWD");
	return (status);
}

static int	do_cd(char *path)
{
	int	status;

	status = chdir(path);
	if (status == -1)
	{
		print_error(path, "No such file or directory");
		return (1);
	}
	return (0);
}

int	ft_cd(char **strs)
{
	int		status;
	char	*path;

	if (strs[1] && (strs[1][0] == '-' && strs[1][1]))
	{
		print_error_and_usage(strs[1], "cd");
		return (2);
	}
	status = get_path(strs[1], &path);
	if (status == -1)
	{
		print_error(path, "not set");
		free(path);
		return (1);
	}
	status = do_cd(path);
	if (!status)
		set_pwd(strs[1]);
	free(path);
	return (status);
}
