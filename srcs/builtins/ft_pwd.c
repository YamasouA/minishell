#include "minishell.h"

void ft_pwd()
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		ft_putstr_fd("error pwd", 2);
		return ;
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
}

int main()
{
	ft_pwd();
}
