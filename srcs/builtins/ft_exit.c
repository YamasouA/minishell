#include "minishell.h"


void	exit_with_str(char **strs)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	ft_putstr_fd("minishell : exit: ", STDERR_FILENO);
	ft_putstr_fd(strs[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	exit(255);
}

void	exit_with_number(char **strs, size_t cnt)
{
	long long n;
	bool	ret;

	if (cnt > 2)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	}
	else
	{
		ret = ft_atol_bool(strs[1], &n);
		if (ret)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			exit(n);
		}
		else
			exit_with_str(strs);
	}
}

void	exit_with_arg(char **strs, size_t cnt)
{
	bool	is_digit;
	size_t	i;

	i = 0;
	is_digit = true;
	if (strs[1][i] == '-' | strs[1][i] == '+')
		i++;
	while (strs[1][i])
	{
		if (!ft_isdigit(strs[1][i]))
		{
			is_digit = false;
			break;
		}
		i++;
	}
	if (is_digit)
		exit_with_number(strs, cnt);
	else
		exit_with_str(strs);
}

int ft_exit(char **strs)//, t_env *envp)
{
	size_t	cnt;

	cnt = 0;
	while (strs[cnt])
		cnt++;
	if (cnt == 1)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit(0);
	}
	else
	{
		exit_with_arg(strs, cnt);
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
