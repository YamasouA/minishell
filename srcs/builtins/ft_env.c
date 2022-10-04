#include "minishell.h"

//envp is temporary placement, replace environ later
void	ft_env(char **strs, char *envp[])
{
	int	i;

	if (strs[1])
	{
		ft_putstr_fd("env: does not support arguments", 2);
		return ;
	}
	i = 0;
	while (envp[i])
	{
		ft_putstr_fd(envp[i++], 1);
		ft_putstr_fd("\n", 1);
	}
}

int main(int argc, char **argv, char *envp[])
{
	char	*strs[2] = {"env"};
	ft_env(strs, envp);
}
