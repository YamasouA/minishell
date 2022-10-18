#include "minishell.h"

void	err_exit(char *msg)
{
	perror(msg);
	exit(2);
}
