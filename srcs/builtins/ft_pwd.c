#include "minishell.h"

int	ft_pwd(char **strs)
{
	char	*pwd;

	if (strs[1] && (strs[1][0] == '-' && strs[1][1]))
	{
		print_error_and_usage(strs[1], "pwd");
		return (2);
	}
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("getcwd error: ");
		return (1);
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}
