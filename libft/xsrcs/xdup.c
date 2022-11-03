#include "libft.h"

int	xdup(int oldfd)
{
	int	ret;

	ret = dup(oldfd);
	error_checker("dup error", ret);
	return (ret);
}

void	xdup2(int oldfd, int newfd)
{
	int	ret;

	ret = dup2(oldfd, newfd);
	error_checker("dup2 error", ret);
}