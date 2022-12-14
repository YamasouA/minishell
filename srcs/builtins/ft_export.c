/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:51:47 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:51:48 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_target_var_value(char *arg, t_env *target_var, int flag)
{
	char	*equal_pos;
	char	*value;
	char	*tmp;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return ;
	value = ft_substr(equal_pos, 1, ft_strlen(equal_pos + 1));
	if (value == NULL)
		err_exit("malloc error: ");
	if (flag == EXPORT_APPEND)
	{
		if (target_var->value)
			target_var->value = ft_joinfree(target_var->value, value);
		else
			target_var->value = ft_strdup(value);
		if (target_var->value == NULL)
			err_exit("malloc error: ");
	}
	else if (flag == EXPORT_NEW)
	{
		tmp = target_var->value;
		target_var->value = value;
		free(tmp);
	}
}

static int	update_env_var(char *arg, int flag)
{
	char	**split_args;
	t_env	*target_var;

	split_args = ft_split(arg, '=');
	if (split_args == NULL)
		err_exit("malloc error: ");
	target_var = search_env(g_sh_var.environ, split_args[0]);
	free_strs(split_args);
	if (target_var)
	{
		update_target_var_value(arg, target_var, flag);
		return (1);
	}
	return (0);
}

static void	push_new_env_var(char *arg)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env) * 1);
	if (new == NULL)
		err_exit("malloc error: ");
	set_data(arg, new);
	add_env(&g_sh_var.environ, new);
}

static int	add_var_to_env(char **args)
{
	size_t	i;
	int		flag;
	int		exit_status;

	exit_status = 0;
	i = 0;
	while (args[++i])
	{
		flag = which_update_flag(&args[i]);
		if (flag == EXPORT_ERROR)
			exit_status = 1;
		if (flag == EXPORT_NEW || flag == EXPORT_APPEND)
		{
			if (!update_env_var(args[i], flag))
				push_new_env_var(args[i]);
		}
	}
	return (exit_status);
}

int	ft_export(char **args)
{
	int	exit_status;

	exit_status = 0;
	if (args[1] && (args[1][0] == '-' && args[1][1]))
		exit_status = print_error_and_usage(args[1], "export");
	else if (args[1] == NULL)
		print_prefix_env(g_sh_var.environ);
	else
		exit_status = add_var_to_env(args);
	return (exit_status);
}
