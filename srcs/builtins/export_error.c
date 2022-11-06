/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta <asouta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:51:11 by shongou           #+#    #+#             */
/*   Updated: 2022/11/05 23:39:38 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_invalid_identifier(char *str)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

int	print_error_and_usage(char *arg, char *cmd_name)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putchar_fd(arg[0], STDERR_FILENO);
	ft_putchar_fd(arg[1], STDERR_FILENO);
	ft_putstr_fd(": options not supported\n", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": usage: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	if (ft_strncmp(cmd_name, "export", 7) == 0)
	{
		ft_putstr_fd(" [name[=value] ...] or export [no arguments]\n", \
				STDERR_FILENO);
	}
	else if (ft_strncmp(cmd_name, "unset", 6) == 0)
		ft_putstr_fd(" [name...]\n", STDERR_FILENO);
	else if (ft_strncmp(cmd_name, "env", 4) == 0)
		ft_putstr_fd(" [no arguments]\n", STDERR_FILENO);
	else if (ft_strncmp(cmd_name, "cd", 3) == 0)
		ft_putstr_fd(" [dir]\n", STDERR_FILENO);
	else if (ft_strncmp(cmd_name, "pwd", 4) == 0)
		ft_putstr_fd("\n", STDERR_FILENO);
	return (2);
}
