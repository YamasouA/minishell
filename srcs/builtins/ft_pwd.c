#include "minishell.h"

int	ft_pwd(char **strs)
{
	char	*pwd;

	if (strs[0] == NULL)
		return (1);
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		ft_putstr_fd("error pwd", 2);
		return (1);
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}

//int main()
//{
//	ft_pwd();
//}
