#include "minishell.h"

int	ft_echo(char **strs)//, t_env *envp)
{
	int	i;

	if (strs[1] == NULL)
	{
		ft_putstr_fd("\n", 1);
	}
	else if (ft_strlen(strs[1]) == 2 && ft_strncmp(strs[1], "-n", 3) == 0)
	{
		i = 2;
		while (strs[i])
		{
			ft_putstr_fd(strs[i++], 1);
			if (strs[i] != NULL)
				ft_putstr_fd(" ", 1);
		}	
	}
	else
	{
		i = 1;
		while (strs[i])
		{
			ft_putstr_fd(strs[i++], 1);
			if (strs[i] != NULL)
				ft_putstr_fd(" ", 1);
		}
		ft_putstr_fd("\n", 1);
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
