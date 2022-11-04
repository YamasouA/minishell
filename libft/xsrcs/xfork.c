#include "libft.h"

pid_t	xfork(void)
{
	pid_t	pid;

	pid = fork();
	error_checker("fork error", pid);
	return (pid);
}
