#include "minishell.h"

#define EXPORT_NONE 0
#define EXPORT_NEW 1
#define EXPORT_APPEND 2
#define EXPORT_ERROR 4

//t_env	*g_environ;

bool	is_valid_var(char *str) // shareable unset func
{
	int	i;
	
	if (!str)
		return (false);
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

void	free_args(char **args)
{
	size_t	i;

	i = 0;
	while (args[i])
	{
		free(args[i++]);
	}
	free(args);
}

int	update_env_var(char *arg, int flag)// t_env *envp
{
	char	*equal_pos;
	char	**split_args;
	char	*value;
	t_env	*target_var;
	char	*tmp;

	split_args = ft_split(arg, '='); // must free
	target_var = search_env(g_environ, split_args[0]);
	free_args(split_args);
//	size_t	i;
//
//	i = 0;
//	while (split_args[i])
//	{
//		free(split_args[i++]);
//	}
//	free(split_args);
//	free(split_args);
	if (target_var)
	{
		equal_pos = ft_strchr(arg, '=');
		if (!equal_pos)
			return (1);
		value = ft_substr(equal_pos, 1, ft_strlen(equal_pos + 1));
		if (flag & EXPORT_APPEND)
		{
			target_var->value = ft_joinfree(target_var->value, value);
		}
		else if (flag & EXPORT_NEW)
		{
			tmp = target_var->value;
			target_var->value = value;
			free(tmp);
		}
		return (1);
	}
	return (0);
}

void	print_invalid_identifier(char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void	push_new_env_var(char *arg)//, t_env *envp)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env) * 1);
	if (new == NULL)
		err_exit("malloc error: ");
	else
		set_data(arg, new);
//	if (g_environ == NULL)
//		add_env(&g_environ, new);
//	else
	add_env(&g_environ, new);
//		add_env(&envp, new);
}

int	is_append_flag(char **key, char *eq_pos)
{
	char	*tmp;

	if (eq_pos && eq_pos != *key && *(eq_pos - 1) == '+' && *key[0] != '+')
	{
		*(eq_pos - 1) = '\0';
		tmp = *key;
		*key = ft_strjoin(*key, eq_pos);
		free(tmp); //The actual argument(key) is taken in malloc, so it is free.
		if (*key == NULL)
			err_exit("malloc error: ");
		return (1);
	}
	return (0);
}

int	which_update_flag(char **key)
{
//	char	**split_args;
	char	*var_name;
	char	*eq_pos;
	int		flag;

	flag = EXPORT_NONE;
	eq_pos = ft_strchr(*key, '=');
//	if (eq_pos)
//	{
	if (is_append_flag(key, eq_pos))
		flag = EXPORT_APPEND;
	else
		flag = EXPORT_NEW;
//	}
//	split_args = ft_split(*key, '=');
//	equal_pos = ft_strchr(arg, '=');
	var_name = ft_substr(*key, 0, eq_pos - key[0]);
//	printf("%s\n",var_name);
//	if (!is_valid_var(split_args[0]))
	if (!is_valid_var(var_name))
	{
		free(var_name);
//		free_args(split_args);
		print_invalid_identifier(*key);
		return (EXPORT_ERROR);
	}
//	free_args(split_args);
	free(var_name);
	return (flag); 
}

int	add_var_to_env(char **args)//, t_env *envp)
{
	int	i;
//	char	*eq_pos;
	int	flag;
	int	exit_status;
	
	exit_status = 0;
	i = 0;
	while (args[++i])
	{
		flag = which_update_flag(&args[i]);
		if (flag == EXPORT_ERROR)
			exit_status = 1;
		if (flag & EXPORT_NEW || flag & EXPORT_APPEND)
		{
			if (!update_env_var(args[i], flag))
				push_new_env_var(args[i]);//, envp);
		}
	}
	return (exit_status);
}

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

int	ft_export(char **args)//, t_env *envp)
{
	int	exit_status;

	exit_status = 0;
	if (args[1])
		exit_status = add_var_to_env(args);//, g_environ);
	else
		print_prefix_env(g_environ);
	return (exit_status);
}

//int	main(int argc, char **argv)
//{
//	t_env	*env;
//	int	exit_status;
//	
//	env = create_env();
//	exit_status = ft_export(argv, env);
//	if (argv[1])
//		print_env(env);
//	printf("\n");
//	printf("exit_status: %d\n", exit_status);
//	return (0);
//}
