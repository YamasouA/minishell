#include "libft.h"

void	error_checker(char *msg, int n)
{
	if (n == -1)
	{
		perror(msg);
		exit(1);
	}
}

void	xunlink(const char *pathname)
{
	int	ret;

	ret = unlink(pathname);
	error_checker("unlink error", ret);
}

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

pid_t	xfork(void)
{
	pid_t	pid;

	pid = fork();
	error_checker("fork error", pid);
	return (pid);
}

void	xclose(int fd)
{
	int	ret;

	ret = close(fd);
	error_checker("close error", ret);
}

int	xopen(char *path, int flags)
{
	int	fd;

	fd = open(path, flags);
	error_checker("open error", fd);
	return (fd);
}

void	xpipe(int pipefd[2])
{
	int	ret;

	ret = pipe(pipefd);
	error_checker("pipe error", ret);
}
