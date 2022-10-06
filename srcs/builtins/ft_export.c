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


int	update_env_var(char *arg, char *envp[], int j, bool append_flag)
{
	char	**split_env;
	char	*equal_pos;
	char	**split_args;
	char	*value;

	split_args = ft_split(arg, '='); // must free
//	i = -1;
//	while (envp[i++])
//	{
	split_env = ft_split(envp[j], '='); //must free
	if (ft_strlen(split_args[0]) == ft_strlen(split_env[0])
		&& ft_strncmp(split_args[0], split_env[0], ft_strlen(split_args[0])) == 0)
	{
		if (append_flag)
		{
			equal_pos = ft_strchr(arg, '=');
			value = ft_substr(equal_pos, 1, ft_strlen(equal_pos + 1));
			envp[j] = ft_strjoin(envp[j], value);
			free(value);
			return (1);
		}
		else
		{
			envp[j] = arg;
			return (1);
		}
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

void	add_var_to_env(char **strs, char **envp)
{
	int	i;
	int	j;
	char	*equal_pos;
	bool	append_flag;
	char	**split_args;

	i = 0;
	while (strs[++i])
	{
		append_flag = 0;
		split_args = ft_split(strs[i], '=');
		if (!is_valid_var(split_args[0]) || *equal_pos == strs[i][0])
		{
			print_invalid_identifier(strs[i++]);
			continue ;	
		}
		equal_pos = ft_strchr(strs[i], '=');
		if (equal_pos)
		{
			if (*equal_pos != strs[i][0] && *(equal_pos - 1) == '+' && strs[i][0] != '+') // cut is_append_env
			{
				*(equal_pos - 1) = '\0';
				strs[i] = ft_strjoin(strs[i], equal_pos); //free strs[i]
				append_flag = 1;
			}
			j = -1;
			while (envp[++j])
			{
				if (update_env_var(strs[i], envp, j, append_flag))
					break ;
			}
			if (envp[j] == NULL)
			{
				envp[j] = strs[i];
				envp[j + 1] = NULL;
			}
		}
	}
}

//add_var_to_env
//while strs[idx]
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

void	print_prefix_env(char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("declare -x %s\n", envp[i++]); //after '=' enclose with a double quotation
	}
}

void	ft_export(char **strs, char *envp[])
{
	if (strs[1])
	{
		add_var_to_env(strs, envp);
	}
	else
	{
		print_prefix_env(envp);
	}
}

int	main(int argc, char **argv, char *envp[])
{
	int	i;

	i = 0;
	ft_export(argv, envp);
	if (argv[1])
	{
		while (envp[i])
		{
			printf("%s\n", envp[i++]);
		}
	}
	return (0);
}
