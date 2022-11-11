/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:52:17 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:52:19 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_envlist(t_env *env)
{
	t_env	*head;
	size_t	cnt;

	cnt = 0;
	head = env;
	while (head != NULL)
	{
		cnt++;
		head = head->next;
	}
	return (cnt);
}

t_env	*env_last(t_env *env)
{
	if (env == NULL)
		return (NULL);
	while (env->next != NULL)
		env = env->next;
	return (env);
}

void	add_env(t_env **env, t_env *new)
{
	t_env	*last;

	if (env == NULL)
		return ;
	if (*env == NULL)
	{
		*env = new;
		return ;
	}
	last = env_last(*env);
	last->next = new;
}

t_env	*env_new(char *key, char *value)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env) * 1);
	if (new == NULL)
		err_exit("malloc error: ");
	new->key = ft_xstrdup(key);
	new->value = ft_xstrdup(value);
	new->next = NULL;
	return (new);
}

void	set_data(char *environ, t_env *new)
{
	char	*eq_pos;

	if (new == NULL)
		return ;
	eq_pos = ft_strchr(environ, '=');
	if (eq_pos == NULL)
	{
		new->key = ft_strdup(environ);
		if (new->key == NULL)
			err_exit("malloc error:");
		new->value = NULL;
	}
	else
	{
		new->key = ft_substr(environ, 0, eq_pos - environ);
		new->value = ft_strdup(eq_pos + 1);
		if (new->key == NULL || new->value == NULL)
			err_exit("malloc error:");
	}
	new->is_env = true;
	new->next = NULL;
}
