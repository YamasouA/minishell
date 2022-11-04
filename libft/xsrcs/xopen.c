#include "libft.h"

int	xopen(char *path, int flags)
{
	int	fd;

	fd = open(path, flags);
	error_checker("open error", fd);
	return (fd);
}
