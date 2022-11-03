#include "libft.h"

void	xclose(int fd)
{
	int	ret;

	ret = close(fd);
	error_checker("close error", ret);
}