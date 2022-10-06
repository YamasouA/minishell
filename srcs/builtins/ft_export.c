#include "minishell.h"

bool	is_valid_var(char *str)
{
	int	i;
	
//	printf("%s\n", str);
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


//int	update_env_var(char *arg, t_env *envp, bool append_flag)
int	update_env_var(char *arg, t_env *envp, int flag)
{
//	char	**split_env;
	char	*equal_pos;
	char	**split_args;
	char	*value;
	t_env	*target_var;
	char	*tmp;

	split_args = ft_split(arg, '='); // must free
//	i = -1;
//	while (envp[i++])
//	{
//	split_env = ft_split(envp[j], '='); //must free
//	if (ft_strlen(split_args[0]) == ft_strlen(split_env[0])
//		&& ft_strncmp(split_args[0], split_env[0], ft_strlen(split_args[0])) == 0)
	target_var = search_env(envp, split_args[0]);
	if (target_var)
	{
		equal_pos = ft_strchr(arg, '=');
		value = ft_substr(equal_pos, 1, ft_strlen(equal_pos + 1));
		if (flag & 2)
			target_var->value = ft_joinfree(target_var->value, value);
		else if (flag & 1)
		{
			tmp = target_var->value;
			target_var->value = value;
			free(tmp);
		}
		return (1);
	}
//	}
	return (0);
}

void	print_invalid_identifier(char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void	push_new_env_var(char *arg, t_env *envp)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env) * 1);
	if (new == NULL)
		return ;
	else
		set_data(arg, new);
	add_env(&envp, new);
}

int	is_append_flag(char **key, char *eq_pos)
{
	char	*tmp;

	if (eq_pos && eq_pos != *key && *(eq_pos - 1) == '+' && *key[0] != '+')
	{
		*(eq_pos - 1) = '\0';
		tmp = *key;
		*key = ft_strjoin(*key, eq_pos);
//		free(tmp); //The actual argument(key) is taken in malloc, so it is free.
		return (1);
	}
	return (0);
}

int	which_update_flag(char **key)
{
	char	**split_args;
	char	*eq_pos;
	int		flag;

	flag = 0;//define EXPORT_NOME
	eq_pos = ft_strchr(*key, '=');
	if (eq_pos)
	{
		if (is_append_flag(key, eq_pos))
			flag = 2; //define EXPORT_APPEND
		else
			flag = 1; //define EXPORT_NEW
	}
	split_args = ft_split(*key, '=');
	if (!is_valid_var(split_args[0]))
	{
		//free split_args
		print_invalid_identifier(*key);
		return (4); //define EXPORT_ERROR
	}
	//free split_args
	return (flag); 
}

int	add_var_to_env(char **args, t_env *envp)
{
	int	i;
	char	*eq_pos;
	int	flag;
	int	exit_status;
//	bool	append_flag;
//	char	**split_args;
	exit_status = 0;
	i = 0;
	while (args[++i])
	{
//		flag = -1;
//		eq_pos = ft_strchr(args[i], '=');
		flag = which_update_flag(&args[i]);
		if (flag == 4)
			exit_status = 1;
//		if (is_append_flag(&args[i], eq_pos))
//			append_flag = 1;
//		split_args = ft_split(args[i], '=');
//		if (!is_valid_var(split_args[0]) || *eq_pos == args[i][0])
//		{
//			print_invalid_identifier(args[i]);
//			continue ;	
//		}
//		if (eq_pos && is_valid_var(split_args[0]))
		if (flag & 1 || flag & 2)
		{
//			if (!update_env_var(args[i], envp, append_flag))
			if (!update_env_var(args[i], envp, flag))
				push_new_env_var(args[i], envp);
		}
//		else
//			print_invalid_identifier(args[i]);
	}
	return (exit_status);
}

//add_var_to_env
//while args[idx]
//	split delimi =
//	if !is_valid_split[0]
//		print error and set_error_status 1, but continue
//	if exist =
//		if = prev is +
//			apeend = 1
//		while env[i]
//			if new_name == env[i]_name
//				if apeend
//					env[i] = env[i] + new_value
//				else
//					env[i] = new_name'='new_value
//			i++
//		if (env[i] == NULL)
//			env[i] = new_name'='new_value
//			env[i + 1] == NULL
//	idx++

void	print_prefix_env(t_env *envp)
{
	while (envp)
	{
		printf("declare -x %s=\"%s\"\n", envp->key, envp->value);
		envp = envp->next;
	}
}

int	ft_export(char **args, t_env *envp)
{
	int	exit_status;

	exit_status = 0;
	if (args[1])
	{
		exit_status = add_var_to_env(args, envp);
	}
	else
	{
		print_prefix_env(envp);
	}
	return (exit_status);
}

int	main(int argc, char **argv)
{
	t_env	*env;
	int	exit_status;
	
	env = create_env();
	exit_status = ft_export(argv, env);
	if (argv[1])
		print_env(env);
	printf("\n");
	printf("exit_status: %d\n", exit_status);
	return (0);
}
