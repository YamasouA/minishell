#include "minishell.h"


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
	while (i < 7)//(int)(sizeof(builtins / sizeof(char *))))
	{
		if (ft_strncmp(cmd->cmd[0], builtins[i], ft_strlen(builtins[i]) + 1)
			== 0)
			return ((*builtin_func[i])(cmd->cmd));
		i++;
	}
	return (1);
}

char	*check_path(char *path)
{
	char	*env_path;
	char	*join_path;
	char	**split;
	size_t	i;

	env_path = search_key(g_environ, "PATH");
	if (env_path == NULL)
		return (NULL);
	split = ft_split(env_path, ':');
	i = 0;
	while (split[i] != NULL)
	{
		join_path = join_with_connector(split[i], path, '/');
		if (join_path == NULL)
			return (NULL);
		if (!access(join_path, X_OK))
			return (join_path);
		i++;
	}
	return (NULL);
}

bool	is_path(char *cmd_name)
{
	if (ft_strchr(cmd_name, '/'))
		return (true);
	return (false);
}

void	print_cmd_not_found_error(char *cmd)
{	
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

void	exec_others(t_cmd *cmd)
{
	char	**envstr;
	char	*path;

	envstr = envlist_to_str(g_environ);
	if (is_path(cmd->cmd[0]))
	{
		if (!access(cmd->cmd[0], X_OK))
			execve(cmd->cmd[0], cmd->cmd, envstr);
		else
			print_cmd_not_found_error(cmd->cmd[0]);
	}
	else
	{
		path = check_path(cmd->cmd[0]);
		if (path == NULL)
		{
			print_cmd_not_found_error(cmd->cmd[0]);
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
//	char	*numstr;
	
//	numstr = ft_ultoa(xorshift());
//	ft_putstr_fd(numstr, 2);
//	redirect_in->file_name = ft_strjoin(TMPFILE, numstr);
//	free(numstr);
	fd = open(redirect_in->file_name, O_CREAT | O_WRONLY | O_TRUNC, 00644); //O_APPEND
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

pid_t	exe_cmd(t_cmd *cmd, int pipe_flag)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) < 0)
		perror("OUT!");
	pid = fork();
	if (pid < 0)
		perror("OUT!");
	if (pid == 0)
	{
		if (pipe_flag != 1)
			exe_process(cmd);
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		exit(exe_process(cmd));
	}
	dup2(fd[0], 0);
	close(fd[1]);
	close(fd[0]);
	return pid;
}

pid_t	exe_terminal_node(t_node *node, int pipe_flag) //t_cmd *cmd is better?
{
	pid_t	pid;

	pid = 0;
	if (!pipe_flag && which_builtin(node->cmd->cmd))
	{
		exe_process(node->cmd);
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

void	exec(t_node *node, int pipe_flag)
{
	int	status;
	int	backup_stdin;
	int	backup_stdout;
	static pid_t	pid;
	pid_t	pid2;
	int	exit_status;

	exit_status = 0;
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
	//	go_through_tree(node, pipe_flag);
		if (node->lhs != NULL)
			exec(node->lhs, 1);
		if (node->rhs != NULL && pipe_flag == 0)
			exec(node->rhs, 2);
		else if (node->rhs != NULL)
			exec(node->rhs, 1);
	}
	if (pipe_flag == 0)
	{
		dup2(backup_stdin, 0);
		dup2(backup_stdout, 1);
		while ((pid2 = waitpid(-1, &status, 0)) != -1)
		{
			if (pid == pid2 && WIFEXITED(status))
			{
				exit_status = WEXITSTATUS(status);
			}
		}
	}
}
