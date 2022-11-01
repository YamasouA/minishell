#include "minishell.h"

void	exit_with_str(char **strs, int i)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	ft_putstr_fd("minishell : exit: ", STDERR_FILENO);
	ft_putstr_fd(strs[i], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	exit(255);
}

void	exit_with_number(char **strs, size_t cnt, size_t i)
{
	long long	n;
	bool		ret;

	if (cnt > i + 1)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	}
	else
	{
		ret = ft_atol_bool(strs[i], &n);
		if (ret)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			exit(n);
		}
		else
			exit_with_str(strs, i);
	}
}

void	exit_with_arg(char **strs, size_t cnt, size_t idx)
{
	bool	is_digit;
	size_t	i;

	i = 0;
	is_digit = true;
	if (strs[idx][i] == '-' || strs[idx][i] == '+')
		i++;
	while (strs[idx][i])
	{
		if (!ft_isdigit(strs[idx][i]))
		{
			is_digit = false;
			break ;
		}
		i++;
	}
	if (is_digit)
		exit_with_number(strs, cnt, idx);
	else
		exit_with_str(strs, idx);
}

void	handle_hyphen(char **strs, int cnt)
{
	if (cnt == 2)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit(g_sh_var.exit_status);
	}
	else
	{
		if (ft_strncmp(strs[2], "-", ft_strlen(strs[2]) + 1) == 0)
			exit_with_str(strs, 2);
		exit_with_arg(strs, cnt, 2);
	}
}

int	ft_exit(char **strs)
{
	size_t	cnt;

	cnt = 0;
	while (strs[cnt])
		cnt++;
	if (cnt == 1)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit(g_sh_var.exit_status);
	}
	else
	{
		if (ft_strncmp("--", strs[1], ft_strlen(strs[1]) + 1) == 0)
			handle_hyphen(strs, cnt);
		else
			exit_with_arg(strs, cnt, 1);
	}
	return (1);
}

//int main()
//{
//	t_env	*envp;
//	//char *strs[] = {"exit", "10"};
//	//char *strs[] = {"exit", "257"};
//	//char *strs[] = {"exit", "-10"};
//	//char *strs[] = {"exit", "-257"};
//	//char *strs[] = {"exit", "-1000"};
//	//char *strs[] = {"exit", "1000"};
//	//char *strs[] = {"exit", "a"};
//	//char *strs[] = {"exit", "12a"};
//	char *strs[] = {"exit", "a12"};
//	//char *strs[] = {"exit", "5", "2", "3"}; // 終了ステータスがよくわからない
//	//char *strs[] = {"exit", "x", "y", "z"};
//	//char *strs[] = {"exit", "5", "y", "z"};
//	//char *strs[] = {"exit", "x", "3", "z"};
//	envp = create_env();
//	ft_exit(strs, envp);
//}
