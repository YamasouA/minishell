#include "minishell.h"

//t_env	*g_environ;

bool	is_valid_unset_args(char *str)
{
	int	i;
	
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i])
	{
		if ((!ft_isalnum(str[i]) && str[i] != '_'))
			return (false);
		i++;
	}
	return (true);
}

void	free_env(t_env **env)
{
	free((*env)->key);
	free((*env)->value);
	free(*env);
	*env = NULL;
}

void	print_unset_error(char *msg)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);	
}

void	del_and_free_env(char *key)//, t_env *envp)
{
	t_env	*prev_env;
	t_env	*cur;

	prev_env = NULL;
	cur = g_environ;
//	cur = envp;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key)+1) == 0)
		{
			if (prev_env)
				prev_env->next = cur->next;
			else if (cur == g_environ)
				g_environ = cur->next;
//				envp = cur->next;
			free_env(&cur);
			return ;
		}
		prev_env = cur;
		cur = cur->next;
	}
}

int	ft_unset(char **keys)//, t_env *envp)
{
	int	i;
	int	exit_status;
	
	exit_status = 0;
	i = 1;
	while (keys[i])
	{
		if (!is_valid_unset_args(keys[i]))
		{
			print_unset_error(keys[i++]);
			exit_status = 1;
			continue ;
		}
		del_and_free_env(keys[i]);//, g_environ);
		i++;
	}
	return (exit_status);
}

//t_env	*search_env(t_env *env, char *key);

//int	main(int argc, char **argv)
//{
//	t_env	*env;
//	int		status;
//
//	env = create_env();
//	printf("\n\e[1;33m==prev unset env_list==\e[0m\n");
//	print_env(env);
//	printf("\n");
//	ft_putstr_fd("\e[1;31m===unset error msg===\e[0m\n", 2);
//	status = ft_unset(argv, env);
//	printf("\n\e[1;33m==after unset env_list==\e[0m\n");
//	print_env(env);
//	printf("\n");
//	printf("exit_status: %d\n", status);
//	return (0);
//}
