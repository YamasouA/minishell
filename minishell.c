#include "minishell.h"
#include <termios.h>
#include <sys/ioctl.h>

#define PROMPT_LENGTH 11

typedef struct termios	t_termios;

t_env *g_environ;
bool	g_signal;

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

int	get_x_pos()
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

int	get_print_start()
{
	int			x;
	t_termios	oldstate;
	t_termios	state;

	tcgetattr(0, &oldstate);
	state = oldstate;
	state.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &state);
	ft_putstr_fd("\e[6n", 0);
	x = get_x_pos();
	tcsetattr(0, TCSANOW, &oldstate);
	return (x - 1);
}

void	free_envlist(t_env *g_environ)
{
	t_env	*tmp;

	while (g_environ != NULL)
	{
		tmp = g_environ;
		g_environ = g_environ->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void minishell(int argc, char **argv)
{
	char *line;
	t_token	*tok;
	t_node	*node;
	bool	heredoc_err;
	g_signal = 0;
	int		x;
	struct winsize	ws;
	int			line_length;

//	signal(SIGINT, signal_handler);
	line_length = 0;
	g_environ = create_env();
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		heredoc_err = 0;
		signal(SIGINT, signal_handler);
		x = get_print_start();
		line = readline("minishell> ");
		signal(SIGINT, SIG_IGN);
		if (line == NULL)// || strlen(line) == 0)
		{
			// free(line);
//			free_envlist(g_environ);
			if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1)
			{
				if(0 < ws.ws_col && ws.ws_col == (int)ws.ws_col)
				{
					line_length = (int)ws.ws_col;
				}
			}
			if (line_length - PROMPT_LENGTH - x < 0)
			{
				x = (line_length - PROMPT_LENGTH - x) * (-1);
				printf("\e[1A\e[%dCexit\n", x);
			}
			else if (line_length - PROMPT_LENGTH - x == 0)
				printf("exit\n");
			else
				printf("\e[1A\e[%dCexit\n", x + PROMPT_LENGTH);
			exit(0);
		}
		if (strlen(line) == 0)
			continue;
		add_history(line);
		tok = lexer(line);
		if (tok == NULL)
			continue ;
		node = parse(tok, &heredoc_err);
		if (g_signal != 0 || node == NULL)
		{
			free(line);
			g_signal = 0;
			continue ;
		}
		node = expansion(node);
		//printf("==EXPANSION==\n");
		//print_node(node, 0);
		exec(node, 0);
		free(line);
		free_token(tok);
		free_node(node);
	}
	argc = 0;
	strlen(*argv);
}
