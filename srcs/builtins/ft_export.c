#include "minishell.h"

//t_env	*g_sh_var.environ;

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

int	ft_export(char **args)
{
	int	exit_status;

	exit_status = 0;
	if (args[1] && !(args[1][0] == '-' && args[1][1]))
		exit_status = add_var_to_env(args);
	else if (args[1] == NULL)
		print_prefix_env(g_sh_var.environ);
	else
		exit_status = print_error_and_usage(args[1], "export");
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
