#include "libft.h"

void	error_checker(char *msg, int n)
{
	if (n == -1)
	{
		perror(msg);
		exit(1);
	}
}
