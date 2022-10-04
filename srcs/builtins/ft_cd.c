#include "minishell.h"

void	ft_cd(char **strs)
{
	chdir(strs[1]);
}

int main(void)
{
	getcwd();
	ft_cd(strs);
	getcwd();
	return (0);
}
