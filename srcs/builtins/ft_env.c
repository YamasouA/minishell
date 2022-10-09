#include "minishell.h"

t_env	*g_environ;

int	ft_env(char **strs)//, t_env *envp)
{
	if (strs[1])
	{
		ft_putstr_fd("env: does not support arguments\n", 2);
		return (1);
	}
	print_env(g_environ);
	return (0);
}

//int main(void)
//{
//	char	*strs[2] = {"env"};
//	t_env *environ;
//
//	environ	= create_env();
//	printf("\n\e[1;33m==ft_env==\e[0m\n");
//	ft_env(strs, environ);
//	printf("\n\e[1;33m==env(originator)==\e[0m\n");
//	system("env");
//	printf("\n\n");
//	{
//	char	*strs[3] = {"env", "abc"};
//
//	environ	= create_env();
//	ft_env(strs, environ);
//	}
//	return (0);
//}
