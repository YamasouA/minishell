#include "minishell.h"
#include <termios.h>
#include <sys/ioctl.h>

#define PROMPT_LENGTH 11

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

void	free_envlist(void)
{
	t_env	*tmp;

	while (g_sh_var.environ != NULL)
	{
		tmp = g_sh_var.environ;
		g_sh_var.environ = g_sh_var.environ->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
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

void	set_signal_handler(int signum, sig_t sighandler)
{
	if (signal(signum, sighandler) == SIG_ERR)
		err_exit("signal error: ");
}

void	init(void)
{
	char	*pwd;

	errno = 0;
	g_sh_var.signal = 0;
	g_sh_var.environ = create_env();
	pwd = getcwd(NULL, 0);
	update_or_add_value(&g_sh_var.environ, "PWD", pwd);
	free(pwd);
	update_or_add_value(&g_sh_var.environ, "OLDPWD", NULL);
}

void	all_free(t_token *tok, t_node *node, char *line)
{
	free(line);
	free_token(tok);
	free_node(node);
}

void	minishell(void)
{
	char	*line;
	t_token	*tok;
	t_node	*node;
	bool	heredoc_err;
	int		x;

	init();
	while (1) //cut func?
	{
		errno = 0;
		heredoc_err = 0;
		set_signal_handler(SIGINT, signal_handler);
		set_signal_handler(SIGQUIT, SIG_IGN);
		x = get_print_start();
		line = readline("minishell> ");
		set_signal_handler(SIGINT, SIG_IGN);
		if (line == NULL)// || strlen(line) == 0)
		{
			// free(line);
//			free_envlist(g_sh_var.environ);
			display_exit(x);
			exit(0); //include display_exit?
		}
		if (ft_strlen(line) == 0)
			continue ;
		add_history(line);
		tok = lexer(line);
		if (tok == NULL)
		{
			free(line);
			continue ;
		}
		node = parse(tok, &heredoc_err);
		if (g_sh_var.signal != 0 || node == NULL)
		{
			free(line);
			g_sh_var.signal = 0;
			continue ;
		}
		node = expansion(node);
		exec(node, 0);
		/*
		free(line); //cut func?
		free_token(tok);
		free_node(node);
		*/
		all_free(tok, node, line);
	}
}
