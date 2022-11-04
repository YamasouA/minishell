#include "minishell.h"
#include <termios.h>
#include <sys/ioctl.h>

t_sh_var	g_sh_var = {};
//t_env	*g_environ;
//bool	g_signal;

static void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signal_handler(int signum, sig_t sighandler)
{
	if (signal(signum, sighandler) == SIG_ERR)
		err_exit("signal error: ");
}

//void	free_envlist(void)
//{
//	t_env	*tmp;
//
//	while (g_sh_var.environ != NULL)
//	{
//		tmp = g_sh_var.environ;
//		g_sh_var.environ = g_sh_var.environ->next;
//		free(tmp->key);
//		free(tmp->value);
//		free(tmp);
//	}
//}

//exit output control
int	get_x_pos(void)
{
	int		i;	
	char	c;
	char	p[32];
	char	*semic_pos;

	i = 0;
	while (read(0, &c, 1) != -1)
	{
		p[i++] = c;
		if (c == 'R')
			break ;
	}
	semic_pos = ft_strchr(p, ';');
	return (ft_atoi(semic_pos + 1));
}

int	get_print_start(void)
{
	int			x;
	t_termios	oldstate;
	t_termios	state;

	if (tcgetattr(0, &oldstate) == -1)
		return (0);
	state = oldstate;
	state.c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(0, TCSANOW, &state) == -1)
		return (0);
	ft_putstr_fd("\e[6n", 0);
	x = get_x_pos();
	if (tcsetattr(0, TCSANOW, &oldstate) == -1)
		return (0);
	return (x - 1);
}

void	display_exit(int x)
{
	struct winsize	ws;
	int				line_length;

	line_length = 0;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) //cut func ?
		return ;
	if (0 < ws.ws_col && ws.ws_col == (int)ws.ws_col)
		line_length = (int)ws.ws_col;
	if (line_length - PROMPT_LENGTH - x < 0)
	{
		x = (line_length - PROMPT_LENGTH - x) * (-1);
		printf("\e[1A\e[%dCexit\n", x);
	}
	else if (line_length - PROMPT_LENGTH - x == 0)
		printf("exit\n");
	else
		printf("\e[1A\e[%dCexit\n", x + PROMPT_LENGTH);
}
//so far

void	init(char **line, t_token **tok, t_node **node)
{
	char	*pwd;

	errno = 0;
	g_sh_var.signal = 0;
	g_sh_var.environ = create_env();
	pwd = getcwd(NULL, 0);
	update_or_add_value(&g_sh_var.environ, "PWD", pwd);
	free(pwd);
	update_or_add_value(&g_sh_var.environ, "OLDPWD", NULL);
	*line = NULL;
	*tok = NULL;
	*node = NULL;
}

char	*readline_wrapper(int *x)
{
	char	*line;

	*x = get_print_start();
	line = readline("minishell> ");
	if (line == NULL)
	{
		display_exit(*x);
		exit(0);
	}
	if (ft_strlen(line) != 0)
		add_history(line);
	return (line);
}

void	loop_init(bool *heredoc_err, char **line, t_token **tok, t_node **node)
{
	set_signal_handler(SIGINT, signal_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
	all_free(*line, *tok, *node);
	errno = 0;
	g_sh_var.signal = 0;
	*heredoc_err = 0;
	*line = NULL;
	*tok = NULL;
	*node = NULL;
}

void	minishell(void)
{
	char	*line;
	t_token	*tok;
	t_node	*node;
	bool	heredoc_err;
	int		x;

	init(&line, &tok, &node);
	while (1)
	{
		loop_init(&heredoc_err, &line, &tok, &node);
		line = readline_wrapper(&x);
		set_signal_handler(SIGINT, SIG_IGN);
		if (ft_strlen(line) == 0)
			continue ;
		tok = lexer(line);
		if (tok == NULL)
			continue ;
		node = parse(&tok, &heredoc_err);
		if (g_sh_var.signal != 0 || node == NULL)
			continue ;
		node = expansion(node);
		exec(node, 0);
	}
}
