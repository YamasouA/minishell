#include "minishell.h"

void ft_pwd()
{
	ft_putendl_fd(getenv("PWD"), STDOUT_FILENO);
}

int main()
{
	ft_pwd();
}
