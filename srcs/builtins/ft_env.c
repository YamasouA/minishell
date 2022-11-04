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
		if (strs[1] && (strs[1][0] == '-' && strs[1][1]))
			print_error_and_usage(strs[1], "env");
		else
			ft_putstr_fd("env: does not support arguments\n", 2);
		return (2);
	}
	print_env(g_sh_var.environ);
	return (0);
}
