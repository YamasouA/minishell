#include "minishell.h"

static int	exec_builtin(t_cmd *cmd)
{
	const char	*builtins[] = {"cd", "echo", "unset", \
		"export", "exit", "pwd", "env"};
	static int	(*builtin_func[])(char **) = {&ft_cd, &ft_echo, &ft_unset, \
		&ft_export, &ft_exit, &ft_pwd, &ft_env};
	size_t		i;

	i = 0;
	while (i < (sizeof(builtins) / sizeof(builtins[0])))
	{
		if (ft_strncmp(cmd->cmd[0], builtins[i], ft_strlen(builtins[i]) + 1)
			== 0)
			return ((*builtin_func[i])(cmd->cmd));
		i++;
	}
	return (1);
}

int	exe_process(t_cmd *cmd)
{
	int	status;

	errno = 0;
	status = 0;
	if (is_redirect(cmd))
	{
		if (do_redirect(cmd))
		{
			return (1);
		}
	}
	if (which_builtin(cmd->cmd))
		status = exec_builtin(cmd);
	else if (cmd->cmd[0] != NULL && cmd->cmd[0][0] != '\0')
		exec_others(cmd);
	return (status);
}

static void	sig_handler(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

pid_t	exe_cmd(t_cmd *cmd, int pipe_flag)
{
	int		fd[2];
	pid_t	pid;

	xpipe(fd);
	set_signal_handler(SIGINT, sig_handler);
	pid = xfork();
	if (pid == 0)
	{
		set_signal_handler(SIGINT, SIG_DFL);
		set_signal_handler(SIGQUIT, SIG_DFL);
		if (pipe_flag != 1)
			exit(exe_process(cmd));
		xdup2(fd[1], 1);
		xclose(fd[1]);
		xclose(fd[0]);
		exit(exe_process(cmd));
	}
	xdup2(fd[0], 0);
	xclose(fd[1]);
	xclose(fd[0]);
	return (pid);
}
