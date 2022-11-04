#include "libft.h"

void	xpipe(int pipefd[2])
{
	int	ret;

	ret = pipe(pipefd);
	error_checker("pipe error", ret);
}
