#include "minishell.h"

//t_env	*g_environ;
//void	print_environ(t_env *env)
//{
//	t_env	*tmp;
//
//	tmp = env;
//	while (tmp)
//	{
//		if (tmp->value)
//		{
//			ft_putstr_fd(tmp->key, 1);
//			ft_putchar_fd('=', 1);
//			ft_putstr_fd(tmp->value, 1);
//			ft_putchar_fd('\n', 1);
//		}
////			printf("%s=%s\n", tmp->key, tmp->value);
//		tmp = tmp->next;
//	}
//}
void	print_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

//int	env_error_and_usage(char *arg, char *cmd_name)
//{
//	print_error_and_usage(arg, cmd_name);
//	ft_putstr_fd(" [no options, no arguments]\n", 2);
//	return (2);
//}

void	print_env(t_env *env) //move env.c ?
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int	ft_env(char **strs)//, t_env *envp)
{
	if (strs[1])
	{
		if (strs[1] && (strs[1][0] == '-' && strs[1][1]))
			print_error_and_usage(strs[1], "env");
		else
			ft_putstr_fd("env: does not support arguments\n", 2);
		return (2); // 1 ?
	}
	print_env(g_sh_var.environ);
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
