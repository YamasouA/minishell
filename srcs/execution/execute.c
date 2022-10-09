#include "minishell.h"

//t_env	*g_environ;

bool	is_redirect(t_cmd *cmd)
{
	if (cmd->redirect_in->next == NULL && cmd->redirect_out->next == NULL)
		return (false);
	return (true);
}

bool	which_builtin(char **cmd)
{
	static char	*builtins[] = {"cd", "echo", "unset", "export", "exit", "pwd", "env"};
	size_t	i;

	i = 0;
	while (builtins[i] != NULL)
	{
		if (ft_strncmp(builtins[i], cmd[0], ft_strlen(builtins[i]) + 1) == 0)
			return (true);
		i++;
	}
	return (false);
}


char	**envlist_to_str(t_env *env)
{
	t_env	*head;
	t_env	*head2;
	size_t	cnt;
	size_t	i;
	char	**envstr;

	cnt = 0;
	head = env;
	head2 = env;
	while (head2 != NULL)
	{
		cnt++;
		head2 = head2->next;
	}
	envstr = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (envstr == NULL)
		perror("OUT");
	i = 0;
	while (head != NULL)
	{
//		envstr[i] = join_slash(head->key, head->value);
		envstr[i] = join_with_connector(head->key, head->value, '=');
		if (envstr[i] == NULL)
			perror("OUT");
		head = head->next;
		i++;
	}
	envstr[i] = NULL;
	return (envstr);
}

int	exec_builtin(t_cmd *cmd)
{
	static char	*builtins[] = {"cd", "echo", "unset", "export", "exit", "pwd", "env"};
	static int (*builtin_func[])(char **) = {&ft_cd, &ft_echo, &ft_unset, &ft_export, &ft_exit, &ft_pwd, &ft_env};
	int			i;

	i = 0;
	while (i < 7)//(int)(sizeof(builtins / sizeof(char *))))
	{
		if (ft_strncmp(cmd->cmd[0], builtins[i], ft_strlen(builtins[i])+1) == 0)
			return (*builtin_func[i])(cmd->cmd);//, g_environ);
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
	//printf("env: %s\n", env_path);
//	printf("ok1\n");
	if (env_path == NULL)
		return (NULL);
	//printf("ok2\n");
	split = ft_split(env_path, ':');
	i = 0;
	while (split[i] != NULL)
	{
//		join_path = slash_join(split[i], path);
		join_path = join_with_connector(split[i], path, '/');
		if (join_path == NULL)
			return (NULL);
		if (!access(join_path, X_OK))
			return (join_path);
		i++;
	}
	printf("ok3\n");
	return (NULL);
}

bool	is_path(char *cmd_name)
{
	if (ft_strchr(cmd_name, '/'))
		return (true);
	return (false);
}

void	exec_others(t_cmd *cmd)
{
	char	**envstr;
	char	*path;

	envstr = envlist_to_str(g_environ);
	if (is_path(cmd->cmd[0]) && !access(cmd->cmd[0], X_OK)) //needs only access?
	{
		execve(cmd->cmd[0], cmd->cmd, envstr);
	}
	else
	{
		path = check_path(cmd->cmd[0]);
		if (path == NULL)
			perror("OUT1");
		execve(path, cmd->cmd, envstr);
	}
}

void	do_redirect(t_cmd *cmd)
{
	t_redirect	*redirect_in;
	t_redirect	*redirect_out;
	int	fd;
	
	fd = -1;
	redirect_in = cmd->redirect_in;
	while (redirect_in->next)
	{
		redirect_in = redirect_in->next;
		fd = open(redirect_in->file_name, O_RDONLY);
		dup2(fd, 0);
		close(fd);
		unlink("here_doc_tmp_file");
	}
	redirect_out = cmd->redirect_out;
	while (redirect_out->next)
	{
		redirect_out = redirect_out->next;
		if (redirect_out->type == APPEND)
			fd = open(redirect_out->file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			fd = open(redirect_out->file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup2(fd, 1);
		close(fd);
	}
}

int	exe_process(t_cmd *cmd)
{
	int	status;

	status = 0;
	if (is_redirect(cmd))
		do_redirect(cmd);
	if (which_builtin(cmd->cmd))
		status = exec_builtin(cmd);
	else
		exec_others(cmd);
	return (status);
}

void	exe_cmd(t_cmd *cmd, int pipe_flag)
{
	int		fd[2];
	pid_t	pid;
	//int backup[2];

	if (pipe(fd) < 0)
		perror("OUT!");
	pid = fork();
	if (pid < 0)
		perror("OUT!");
	//printf("cmd: %s\n", cmd->cmd[0]);
	if (pid == 0)
	{
		if (pipe_flag == 2)
			exe_process(cmd);
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		exit(exe_process(cmd));
	}
	else
	{
		if (pipe_flag == 1)
		{
			dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
		}
	}
}

void	exec(t_node *node, int pipe_flag)
{
	int	status;
	pid_t	pid;
	int	backup_fd;
	
	backup_fd = dup(0);
	if (node->lhs == NULL && node->rhs == NULL)
	{
		if (!pipe_flag && which_builtin(node->cmd->cmd))
		{
			exe_process(node->cmd);
		}
		else if (node->cmd->cmd[0] != NULL)
		{
			exe_cmd(node->cmd, pipe_flag);
		}
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			exec(node->lhs, 1);
//		if (node->lhs != NULL && node->lhs->kind != ND_PIPE)
//			exec(node->lhs, 0);
		if (node->rhs != NULL && pipe_flag == 0)
			exec(node->rhs, 2);
		if (node->rhs != NULL)
			exec(node->rhs, 1);
	}
	if (pipe_flag == 0)
		dup2(backup_fd, 0);
	pid = waitpid(-1, &status, 0);
	//pid = 1;
	//while (pid > 0)
	//{
	//	pid = waitpid(-1, &status, 0);
	//	printf("pid%d\n", pid);
	//}
//	sleep(1);
}
