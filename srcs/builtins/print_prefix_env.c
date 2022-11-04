/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_prefix_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:52:05 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:52:06 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_prefix_env(t_env *envp)
{
	while (envp)
	{
		if (envp->value)
			printf("declare -x %s=\"%s\"\n", envp->key, envp->value);
		else
			printf("declare -x %s\n", envp->key);
		envp = envp->next;
	}
}
