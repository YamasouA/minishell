#include "minishell.h"

void	ft_env(char **strs, t_env *envp)
{
	if (strs[1])
	{
		ft_putstr_fd("env: does not support arguments\n", 2);
		return ;
	}
	print_env(envp);
}

int main(void)
{
	char	*strs[2] = {"env"};
	t_env *environ;

	environ	= create_env();
	ft_env(strs, environ);
	printf("\n\n");
	{
	char	*strs[3] = {"env", "abc"};

	environ	= create_env();
	ft_env(strs, environ);
	}
	return (0);
}
