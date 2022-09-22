#include "minishell.h"

void	ft_exit(char *msg)
{
	printf("%s\n", msg);
	exit(EXIT_FAILURE);
}
