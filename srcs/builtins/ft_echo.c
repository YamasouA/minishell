#include "minishell.h"

void	ft_echo(char **strs)
{
	int	i;

	if (ft_strlen(strs[1]) == 2 && ft_strncmp(strs[1], "-n", 3) == 0)
	{
		i = 2;
		while (strs[i])
		{
			ft_putstr_fd(strs[i++], 1);
		}	
	}
	else
	{
		i = 1;
		while (strs[i])
		{
			ft_putstr_fd(strs[i++], 1);
		}
		ft_putstr_fd("\n", 1);
	}
}

int main(void)
{
//	char *strs[5] = {"echo", "-n", "abc", NULL};
	char *strs[4] = {"echo", "abc", NULL};

	ft_echo(strs);
	return (0);
}
