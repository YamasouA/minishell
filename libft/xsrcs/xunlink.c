#include "libft.h"

void	xunlink(const char *pathname)
{
	int	ret;

	ret = unlink(pathname);
	error_checker("unlink error", ret);
}
