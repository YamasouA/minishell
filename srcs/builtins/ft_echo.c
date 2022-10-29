#include "minishell.h"

void	echo_without_newline(char **strs, int i)
{
	while (strs[i])
	{
		ft_putstr_fd(strs[i++], 1);
		if (strs[i] != NULL)
			ft_putstr_fd(" ", 1);
	}			
}

void	echo_with_option_n(char **strs)
{
	int	i;

	i = 2;
	echo_without_newline(strs, i);
}

void	echo_no_option(char **strs)
{
	int	i;

	i = 1;
	echo_without_newline(strs, i);
	ft_putstr_fd("\n", 1);
}

int	ft_echo(char **strs)//, t_env *envp)
{
	if (strs[1] == NULL)
		ft_putstr_fd("\n", 1);
	else if (ft_strlen(strs[1]) == 2 && ft_strncmp(strs[1], "-n", 3) == 0)
	{
		echo_with_option_n(strs);
	}
	else
	{
		echo_no_option(strs);
	}
	return (0);
}

//int main(void)
//{
////	char *strs[5] = {"echo", "-n", "abc", NULL};
//	char *strs[4] = {"echo", "abc", NULL};
//	t_env	*envp;
//
//	envp = create_env();
//	ft_echo(strs, envp);
//	return (0);
//}
