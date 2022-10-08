#include "minishell.h"

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
		envstr[i] = join_slash(head->key, head->value);
		if (envstr[i] == NULL)
			perror("OUT");
		i++;
	}
	return (envstr);
}

void	exec_builtin(t_cmd *cmd)
{
	static char	*builtins[] = {"cd", "echo", "unset", "export", "exit", "pwd", "env"};
	static int (*builtin_func[])(char **) = {&ft_cd, &ft_echo, &ft_unset, &ft_export, &ft_exit, &ft_pwd, &ft_env};
	int			i;

	i = 0;
	while (i < sizeof(builtin_str / sizeof(char *)))
	{
		if (ft_strncmp(cmd->cmd[0], builtin_str[i], ft_strlen(builtin_str[i]) == 0))
			return (*builtin_func[i](cmd->cmd);
	}
}

char	*check_path(char *path)
{
	char	*env_path;
	char	*join_path;
	char	**split;
	size_t	i;

	env_path = search_key("PATH");
	if (env_path == NULL)
		return (NULL);
	split = ft_split(env_path, ':');
	i = 0;
	while (split[i] != NULL)
	{
		join_path = slash_join(split[i], path);
		if (join_path == NULL)
			return (NULL);
		if (!access(join_path, X_OK))
			return (join_path);
		i++;
	}

	return (NULL);
}

void	exec_others(t_cmd *cmd)
{
	char	**envstr;
	char	*path;

	envstr = envlist_to_str(env);
	if (is_path(cmd->cmd) && !access(cmd->cmd, X_OK))
	{
		execve(cmd->cmd[0], cmd->cmd, envstr);
	}
	else
	{
		path = check_path(cmd->cmd[0]);
		if (path == NULL)
			perror("OUT");
		execve(path, cmd->cmd, envstr);
	}
}

void	exe_process(t_cmd *cmd)
{
	if (is_redirect(cmd))
		do_redirect(cmd);
	if (is_builtins(cmd->cmd))
		exec_builtin(cmd);
	else
		exec_others(cmd);
}

void	exe_cmd(t_cmd *cmd)
{
	int	fd[2];
	pid_t	pid;

	if (pipe(fd) < 0)
		perror("OUT!");
	pid = fork();
	if (pid < 0)
		perror("OUT!");
	if (pid == 0)
	{
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		exe_process(cmd);
	}
	else
	{
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		return ;
	}
}

void	exec(t_node *node, int pipe_flag)
{
	if (node->lhs == NULL && node->rhs == NULL)
	{
		if (!pipe_flag && is_builtins(node->cmd->cmd))
		{
			exe_process(node->cmd);
		}
		else if (node->cmd->cmd[0] != NULL)
		{
			exe_cmd(node->cmd);
		}
	}
	if (node->lhs != NULL || node->rhs != NULL)
	{
		if (node->lhs != NULL)
			exec(node->lhs, 1);
		if (node->rhs != NULL)
			exec(node->rhs, 1);
	}
	waitpid(-1, &WEXITSTATUS, 0);
}
