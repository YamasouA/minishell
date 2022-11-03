#include "minishell.h"

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

bool	is_redirect(t_cmd *cmd)
{
	if (cmd->redirect_in->next == NULL && cmd->redirect_out->next == NULL)
		return (false);
	return (true);
}

bool	which_builtin(char **cmd)
{
	static char	*builtins[] = {"cd", "echo", "unset", \
		"export", "exit", "pwd", "env", NULL};
	size_t		i;

	if (*cmd == NULL)
		return (false);
	i = 0;
	while (builtins[i] != NULL)
	{
		if (ft_strncmp(builtins[i], cmd[0], ft_strlen(builtins[i]) + 1) == 0)
			return (true);
		i++;
	}
	return (false);
}

//
size_t	count_envlist(t_env *env)
{
	t_env	*head;
	size_t	cnt;

	cnt = 0;
	head = env;
	while (head != NULL)
	{
		cnt++;
		head = head->next;
	}
	return (cnt);
}

char	**envlist_to_str(t_env *env)
{
	t_env	*head;
	size_t	cnt;
	size_t	i;
	char	**envstr;

	cnt = count_envlist(env);
	envstr = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (envstr == NULL)
		err_exit("malloc error: ");
	i = -1;
	head = env;
	while (head != NULL)
	{
		if (head->value)
			envstr[++i] = join_with_connector(head->key, head->value, '=');
		if (envstr[i] == NULL)
			err_exit("malloc error: ");
		head = head->next;
	}
	envstr[++i] = NULL;
	return (envstr);
}
//

//
char	*create_and_check_path(char *env_path, char *cmd_name)
{
	char	*join_path;

	join_path = join_with_connector(env_path, cmd_name, '/');
	if (join_path == NULL)
		err_exit("malloc error: ");
	if (!access(join_path, X_OK))
		errno = 0;
	return (join_path);
}

char	*check_path_list(char **env_path, char *cmd)
{
	int		i;
	char	*join_path;
	char	*save_error_path;

	save_error_path = NULL;
	i = -1;
	while (env_path[++i] != NULL)
	{
		join_path = create_and_check_path(env_path[i], cmd);
		if (errno == 0)
		{
			free(save_error_path);
			return (join_path);
		}
		else if (errno == EACCES)
		{
			free(save_error_path);
			save_error_path = ft_strdup(join_path);
			if (save_error_path == NULL)
				err_exit("malloc error: ");
		}
		free(join_path);
	}
	return (save_error_path);
}

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
		free(strs[i++]);
	free(strs);
}

char	*check_path(char *path)
{
	char	*env_path;
	char	*join_path;
	char	**split_path;

	env_path = search_key(g_sh_var.environ, "PATH");
	if (env_path == NULL)
		return (NULL);
	split_path = ft_split(env_path, ':');
	join_path = check_path_list(split_path, path);
	free_strs(split_path);
	if (errno == 0)
		return (join_path);
	errno = ENOENT;
	if (join_path)
		errno = EACCES;
	return (join_path);
}
//

bool	is_path(char *cmd_name)
{
	if (ft_strchr(cmd_name, '/'))
		return (true);
	return (false);
}

bool	is_directory(char *pathname)
{
	struct stat	sb;

	ft_memset(&sb, 0, sizeof(struct stat));
	stat(pathname, &sb);
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
		return (true);
	return (false);
}

void	exec_by_cmd_path(t_cmd *cmd, char **envstr)
{
	if (is_directory(cmd->cmd[0]))
	{
		//ft_putstr_fd("minishell: ", 2);
		//ft_putstr_fd(cmd->cmd[0], 2);
		//ft_putstr_fd(": is a directory\n", 2);
		err_is_a_directory(cmd->cmd[0]);
		exit(126);
	}
	else if (!access(cmd->cmd[0], X_OK))
	{
		execve(cmd->cmd[0], cmd->cmd, envstr);
		err_exit("execve error: ");
	}
	else
		print_access_err(cmd->cmd[0]);
}

void	exec_by_cmd_name(t_cmd *cmd, char **envstr)
{
	char	*path;

	path = check_path(cmd->cmd[0]);
	if (path == NULL || errno != 0)
	{
		if (errno == EACCES)
			print_access_err(path);
		else if (errno == ENOENT)
			print_access_err(cmd->cmd[0]);
		else if (path == NULL) //&& errno == 0)
			print_access_err(cmd->cmd[0]);
//			perror("OUT1");
	}
//		exit(execve(path, cmd->cmd, envstr));
	execve(path, cmd->cmd, envstr);
	err_exit("execve error: ");
}

pid_t	exe_terminal_node(t_node *node, int pipe_flag) //t_cmd *cmd is better?
{
	pid_t	pid;

	pid = 0;
	if (!pipe_flag && which_builtin(node->cmd->cmd))
	{
		g_sh_var.exit_status = exe_process(node->cmd);
	}
	else
//	else if (node->cmd->cmd[0] != NULL)
	{
		pid = exe_cmd(node->cmd, pipe_flag);
	}
	return (pid);
}

void	go_through_tree(t_node *node, int pipe_flag)
{
	if (node->lhs != NULL)
		exec(node->lhs, 1);
	if (node->rhs != NULL && pipe_flag == 0)
		exec(node->rhs, 2);
	else if (node->rhs != NULL)
		exec(node->rhs, 1);
}

void	get_exit_status(pid_t pid)
{
	int		status;
	pid_t	pid2;

	pid2 = 0;
	while (pid2 != -1)
	{
		pid2 = waitpid(-1, &status, 0);
		if (pid == pid2)
		{
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					ft_putendl_fd("Quit: 3", 2);
				//else if(WTERMSIG(status) == SIGINT)
					//ft_putendl_fd("", 2);
				g_sh_var.exit_status = WTERMSIG(status) + 128;
			}
			else if (WIFEXITED(status))
				g_sh_var.exit_status = WEXITSTATUS(status);
		}
	}
}

void	exec(t_node *node, int pipe_flag)
{
	int				backup_stdin;
	int				backup_stdout;
	static pid_t	pid;

	errno = 0;
	//g_sh_var.exit_status = 0;
	if (pipe_flag == 0)
	{
		backup_stdin = xdup(0);
		backup_stdout = xdup(1);
	}
	if (node->lhs == NULL && node->rhs == NULL)
		pid = exe_terminal_node(node, pipe_flag);
	if (node->lhs != NULL || node->rhs != NULL)
		go_through_tree(node, pipe_flag);
	if (pipe_flag == 0)
	{
		xdup2(backup_stdin, 0);
		xdup2(backup_stdout, 1);
		xclose(backup_stdin);
		xclose(backup_stdout);
		get_exit_status(pid);
	}
}
