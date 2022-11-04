/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:51:58 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:51:59 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_unset_args(char *str)
{
	int	i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i])
	{
		if ((!ft_isalnum(str[i]) && str[i] != '_'))
			return (false);
		i++;
	}
	return (true);
}

static void	free_env(t_env **env)
{
	free((*env)->key);
	free((*env)->value);
	free(*env);
	*env = NULL;
}

static void	print_unset_error(char *msg)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static void	del_and_free_env(char *key)
{
	t_env	*prev_env;
	t_env	*cur;

	prev_env = NULL;
	cur = g_sh_var.environ;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev_env)
				prev_env->next = cur->next;
			else if (cur == g_sh_var.environ)
				g_sh_var.environ = cur->next;
			free_env(&cur);
			return ;
		}
		prev_env = cur;
		cur = cur->next;
	}
}

int	ft_unset(char **keys)
{
	int	i;
	int	exit_status;

	if (keys[1] && (keys[1][0] == '-' && keys[1][1]))
	{
		exit_status = print_error_and_usage(keys[1], "unset");
		return (exit_status);
	}
	exit_status = 0;
	i = 1;
	while (keys[i])
	{
		if (!is_valid_unset_args(keys[i]))
		{
			print_unset_error(keys[i++]);
			exit_status = 1;
			continue ;
		}
		del_and_free_env(keys[i]);
		i++;
	}
	return (exit_status);
}
