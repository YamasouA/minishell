#include "minishell.h"


void	exit_with_number(char **strs, size_t cnt)
{
	int	n;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (cnt > 2)
	{
		ft_putendl_fd("bash: exit: too many arguments", STDERR_FILENO);
	}
	else
	{
		n = ft_atoi(strs[1]);
		if (n < 0)
			exit(256 + n);
		else if (n <= 255)
			exit(n);
		else
			exit(n - 256);
	}
}

void	exit_with_str(char **strs)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	ft_putstr_fd("minishell : exit: ", STDERR_FILENO);
	ft_putstr_fd(strs[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	exit(255);
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

void my_exit(char **strs)
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
}

int main()
{
	//char *strs[] = {"exit", "10"};
	//char *strs[] = {"exit", "257"};
	//char *strs[] = {"exit", "-10"};
	//char *strs[] = {"exit", "-257"};
	//char *strs[] = {"exit", "-1000"};
	//char *strs[] = {"exit", "a"};
	//char *strs[] = {"exit", "12a"};
	char *strs[] = {"exit", "5", "2", "3"}; // 終了ステータスがよくわからない
	//char *strs[] = {"exit", "x", "y", "z"};
	//char *strs[] = {"exit", "5", "y", "z"};
	//char *strs[] = {"exit", "x", "3", "z"};
	my_exit(strs);
}
