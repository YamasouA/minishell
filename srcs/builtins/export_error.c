#include "minishell.h"

void	print_invalid_identifier(char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	print_error_and_usage(char *arg, char *cmd_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putchar_fd(arg[0], 2);
	ft_putchar_fd(arg[1], 2);
	ft_putstr_fd(": options not supported\n", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": usage: ", 2);
	ft_putstr_fd(cmd_name, 2);
	if (ft_strncmp(cmd_name, "export", 7) == 0)
		ft_putstr_fd(" [name[=value] ...] or export [no arguments]\n", 2);
	else if (ft_strncmp(cmd_name, "unset", 6) == 0)
		ft_putstr_fd(" [name...]\n", 2);
	else if (ft_strncmp(cmd_name, "env", 4) == 0)
		ft_putstr_fd(" [no arguments]\n", 2);
	else if (ft_strncmp(cmd_name, "cd", 3) == 0)
		ft_putstr_fd(" [dir]\n", 2);
	else if (ft_strncmp(cmd_name, "pwd", 4) == 0)
		ft_putstr_fd("\n", 2);
	return (2);
}
