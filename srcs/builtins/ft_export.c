#include "minishell.h"

#define EXPORT_NONE 0
#define EXPORT_NEW 1
#define EXPORT_APPEND 2
#define EXPORT_ERROR 4

//t_env	*g_sh_var.environ;

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

void	free_args(char **args) //same free_strs
{
	size_t	i;

	i = 0;
	while (args[i])
	{
		free(args[i++]);
	}
	free(args);
}

void	update_target_var_value(char *arg, t_env *target_var, int flag)
{	
	char	*equal_pos;
	char	*value;
	char	*tmp;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return ;
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
	return ;
}

int	update_env_var(char *arg, int flag)
{
	char	**split_args;
	t_env	*target_var;

	split_args = ft_split(arg, '=');
	target_var = search_env(g_sh_var.environ, split_args[0]);
	free_args(split_args);
	if (target_var)
	{
		update_target_var_value(arg, target_var, flag);
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

void	push_new_env_var(char *arg)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env) * 1);
	if (new == NULL)
		err_exit("malloc error: ");
	else
		set_data(arg, new);
	add_env(&g_sh_var.environ, new);
}

int	is_append_flag(char **key, char *eq_pos)
{
	char	*tmp;

	if (eq_pos && eq_pos != *key && *(eq_pos - 1) == '+' && *key[0] != '+')
	{
		*(eq_pos - 1) = '\0'; //move below if (is_append_flag) ?
		tmp = *key;
		*key = ft_strjoin(*key, eq_pos);
		free(tmp);
		if (*key == NULL)
			err_exit("malloc error: ");
		return (1);
	}
	return (0);
}

int	which_update_flag(char **key)
{
	char	*var_name;
	char	*eq_pos;
	int		flag;

	flag = EXPORT_NONE;
	eq_pos = ft_strchr(*key, '=');
	if (is_append_flag(key, eq_pos))
	{
		flag = EXPORT_APPEND;
		eq_pos = ft_strchr(*key, '=');
		var_name = ft_substr(*key, 0, eq_pos - key[0]);
	}
	else
	{
		flag = EXPORT_NEW;
		var_name = ft_substr(*key, 0, eq_pos - key[0]);
	}
	if (!is_valid_var(var_name))
	{
		free(var_name);
		print_invalid_identifier(*key);
		return (EXPORT_ERROR);
	}
	free(var_name);
	return (flag);
}

int	add_var_to_env(char **args)
{
	int	i;
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
				push_new_env_var(args[i]);
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

int	ft_export(char **args)
{
	int	exit_status;

	exit_status = 0;
	if (args[1])
		exit_status = add_var_to_env(args);
	else
		print_prefix_env(g_sh_var.environ);
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
