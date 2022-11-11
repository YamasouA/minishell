/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta <asouta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:51:36 by shongou           #+#    #+#             */
/*   Updated: 2022/11/05 23:41:14 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int	ft_env(char **strs)
{
	if (strs[1])
	{
		if (strs[1][0] == '-' && strs[1][1])
			print_error_and_usage(strs[1], "env");
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd("env: does not support arguments\n", STDERR_FILENO);
			ft_putstr_fd("env", STDERR_FILENO);
			ft_putstr_fd(": usage: ", STDERR_FILENO);
			ft_putstr_fd("env", STDERR_FILENO);
			ft_putstr_fd(" [no arguments]\n", STDERR_FILENO);
		}
		return (2);
	}
	print_env(g_sh_var.environ);
	return (0);
}
