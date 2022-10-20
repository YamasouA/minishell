#include "minishell.h"

int	g_exit_status;

bool	is_redirect(t_cmd *cmd)
{
	if (cmd->redirect_in->next == NULL && cmd->redirect_out->next == NULL)
		return (false);
	return (true);
}

bool	which_builtin(char **cmd)
{
	static char	*builtins[] = {"cd", "echo", "unset", \
		"export", "exit", "pwd", "env"};
	size_t		i;

	i = 0;
	while (builtins[i] != NULL)
	{
		if (ft_strncmp(builtins[i], cmd[0], ft_strlen(builtins[i]) + 1) == 0)
			return (true);
		i++;
	}
	return (false);
}

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
		perror("OUT");
	i = -1;
	head = env;
	while (head != NULL)
	{
		envstr[++i] = join_with_connector(head->key, head->value, '=');
		if (envstr[i] == NULL)
			perror("OUT");
		head = head->next;
	}
	envstr[i] = NULL;
	return (envstr);
}

int	exec_builtin(t_cmd *cmd)
{
	static char	*builtins[] = {"cd", "echo", "unset", \
		"export", "exit", "pwd", "env"};
	static int	(*builtin_func[])(char **) = {&ft_cd, &ft_echo, &ft_unset, \
		&ft_export, &ft_exit, &ft_pwd, &ft_env};
	int			i;

	i = 0;
	while (i < 7) //(int)(sizeof(builtins / sizeof(char *))))
	{
		if (ft_strncmp(cmd->cmd[0], builtins[i], ft_strlen(builtins[i]) + 1)
			== 0)
			return ((*builtin_func[i])(cmd->cmd));
		i++;
	}
	return (1);
}

char	*create_and_check_path(char *env_path, char *cmd_name)
{
	char	*join_path;
	
	join_path = join_with_connector(env_path, cmd_name, '/');
	if (join_path == NULL)
		return (NULL);
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
			save_error_path = join_path;
		}
		free(join_path);
	}
	return (save_error_path);
}

char	*check_path(char *path)
{
	char	*env_path;
	char	*join_path;
	char	**split_path;

	env_path = search_key(g_environ, "PATH");
	if (env_path == NULL)
		return (NULL);
	split_path = ft_split(env_path, ':');
	join_path = check_path_list(split_path, path);
	if (errno == 0)
		return (join_path);
	errno = ENOENT;
	if (join_path)
		errno = EACCES;
	return (join_path);
}

bool	is_path(char *cmd_name)
{
	if (ft_strchr(cmd_name, '/'))
		return (true);
	return (false);
}

//void	print_cmd_not_found_error(char *cmd)
//{	
//	ft_putstr_fd("minishell: ", 2);
//	ft_putstr_fd(cmd, 2);
//	ft_putstr_fd(": command not found\n", 2);
////	exit(errno);
//	exit(127);
//}
//
//void	print_permission_denied_error(char *cmd)
//{	
//	ft_putstr_fd("minishell: ", 2);
//	ft_putstr_fd(cmd, 2);
//	ft_putstr_fd(": Permission denied\n", 2);
////	exit(errno);
//	exit(126);
//}

void	print_exec_process_error(char *cmd, char *msg, int status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	if (status == EACCES)
		exit(126);
	else if (status == ENOENT)
		exit(127);
}

void	print_access_err(char *msg)
{
	if (errno == EACCES)
		print_exec_process_error(msg, ": Permission_denied", EACCES);
	else if (errno == ENOENT)
		print_exec_process_error(msg, ": command not found", ENOENT);
}

bool	is_directory(char *pathname)
{
	struct stat	sb;

	stat(pathname, &sb);
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
		return (true);
	return (false);
}

void	exec_others(t_cmd *cmd)
{
	char	**envstr;
	char	*path;

	envstr = envlist_to_str(g_environ);
	if (is_path(cmd->cmd[0]))
	{
		if (is_directory(cmd->cmd[0]))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->cmd[0], 2);
			ft_putstr_fd(": is a directory\n", 2);
		}
		else if (!access(cmd->cmd[0], X_OK))
		{
//			ft_putstr_fd("ok\n", 2);
			execve(cmd->cmd[0], cmd->cmd, envstr);
		}
		else
			print_access_err(cmd->cmd[0]);
	}
	else
	{
		path = check_path(cmd->cmd[0]);
		if (path == NULL || errno != 0)
		{
			if (errno == EACCES)
				print_access_err(path);
			else if (errno == ENOENT)
				print_access_err(cmd->cmd[0]);
//			perror("OUT1");
		}
		exit(execve(path, cmd->cmd, envstr));
	}
}

void	open_and_dup2(t_redirect *redirect)
{
	int	fd;

	if (redirect->type == APPEND)
	{
		fd = open(redirect->file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
		dup2(fd, 1);
	}
	else if (redirect->type == REDIRECT_OUT)
	{
		fd = open(redirect->file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup2(fd, 1);
	}
	else
	{
		fd = open(redirect->file_name, O_RDONLY);
		dup2(fd, 0);
		if (redirect->type == HEREDOC)
			unlink(redirect->file_name);
	}
	close(fd);
}

void	create_heredoc_tmpfile(t_redirect *redirect_in)
{
	int		fd;
//	int		flag;
//	char	*numstr;

//	numstr = ft_ultoa(xorshift());
//	ft_putstr_fd(numstr, 2);
//	redirect_in->file_name = ft_strjoin(TMPFILE, numstr);
//	free(numstr);
//	flag = O_CREAT | O_WRONLY | O_TRUNC;
	fd = open(redirect_in->file_name, O_CREAT | O_WRONLY | O_TRUNC, 00644);
//	usleep(500);
//	printf("doc:%s\n", redirect_in->documents);
	ft_putstr_fd(redirect_in->documents, fd);
	free(redirect_in->documents);
	redirect_in->documents = NULL;
	close(fd);
}

void	do_redirect(t_cmd *cmd)
{
	t_redirect	*redirect_in;
	t_redirect	*redirect_out;

	redirect_in = cmd->redirect_in;
	while (redirect_in->next)
	{
		redirect_in = redirect_in->next;
		if (redirect_in->type == HEREDOC)
		{
			create_heredoc_tmpfile(redirect_in);
		}
		open_and_dup2(redirect_in);
	}
	redirect_out = cmd->redirect_out;
	while (redirect_out->next)
	{
		redirect_out = redirect_out->next;
		open_and_dup2(redirect_out);
	}
}

int	exe_process(t_cmd *cmd)
{
	int	status;

//	ft_putstr_fd("ok\n", 1);
	errno = 0;
	status = 0;
	if (is_redirect(cmd))
	{
		do_redirect(cmd);
	}
	if (which_builtin(cmd->cmd))
		status = exec_builtin(cmd);
	else
		exec_others(cmd);
	return (status);
}

static void	sig_handler(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
}

pid_t	exe_cmd(t_cmd *cmd, int pipe_flag)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) < 0)
		perror("OUT!");
	signal(SIGINT, sig_handler);
	pid = fork();
	if (pid < 0)
		perror("OUT!");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (pipe_flag != 1)
			exit(exe_process(cmd));
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		exit(exe_process(cmd));
	}
	dup2(fd[0], 0);
	close(fd[1]);
	close(fd[0]);
	return (pid);
}

pid_t	exe_terminal_node(t_node *node, int pipe_flag) //t_cmd *cmd is better?
{
	pid_t	pid;

	pid = 0;
	if (!pipe_flag && which_builtin(node->cmd->cmd))
	{
		g_exit_status = exe_process(node->cmd);
	}
	else if (node->cmd->cmd[0] != NULL)
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
				g_exit_status = WTERMSIG(status) + 128;
			}
			else if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
		}
	}
}

void	exec(t_node *node, int pipe_flag)
{
	int				backup_stdin;
	int				backup_stdout;
	static pid_t	pid;

	errno = 0;
	g_exit_status = 0;
	if (pipe_flag == 0)
	{
		backup_stdin = dup(0);
		backup_stdout = dup(1);
	}
	if (node->lhs == NULL && node->rhs == NULL)
	{
		pid = exe_terminal_node(node, pipe_flag);
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		go_through_tree(node, pipe_flag);
//		if (node->lhs != NULL)
//			exec(node->lhs, 1);
//		if (node->rhs != NULL && pipe_flag == 0)
//			exec(node->rhs, 2);
//		else if (node->rhs != NULL)
//			exec(node->rhs, 1);
	}
	if (pipe_flag == 0)
	{
		dup2(backup_stdin, 0);
		dup2(backup_stdout, 1);
		get_exit_status(pid);
//		printf("ex_st: %d\n", g_exit_status);
	}
}
