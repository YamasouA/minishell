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
//	while (1)
//	{
//		read(0, &c, 1);
//		if(c == ';')
//		{
//			while (1)
//			{
//				read(0, &c, 1);
//				if ('0' <= c && c <= '9')
//					p[i++] = c;
//				else
//					break ;
//			}
//		}
//		if(c == 'R')
//			break ;	
//	}
//	p[i] = '\0';
	return (ft_atoi(semic_pos + 1));
//	return (ft_atoi(p));
}

int	get_print_start()
{
	int			x;
	t_termios	oldstate;
	t_termios	state;
//	struct winsize	ws;
//	int			line_length;

//	line_length = 0;
	tcgetattr(0, &oldstate);
	state = oldstate;
	state.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &state);
	ft_putstr_fd("\e[6n", 0);
	x = get_x_pos();
//    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1)
//	{
//		printf("terminal_width  =%d\n", ws.ws_col);
//		printf("terminal_height =%d\n", ws.ws_row);
//        if( 0 < ws.ws_col && ws.ws_col == (int)ws.ws_col )
//		{
//          line_length = (int)ws.ws_col;
//		}
//	}
//	if (line_length - 11 - x < 0)
//	{
//		x = (line_length - 11 - x) * (-1);
//	}
	tcsetattr(0, TCSANOW, &oldstate);
//	printf("%d", x);
//	printf("terminal_width  =%d\n", ws.ws_col);
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
//	char	*num;
//	char	*tmp;
//	char	*str;

//	num = NULL;
//	signal(SIGINT, signal_handler);
	line_length = 0;
	g_environ = create_env();
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		heredoc_err = 0;
		signal(SIGINT, signal_handler);
//		ft_putstr_fd("minishell> ", 1);
		x = get_print_start();
//		printf("%d",rl_point);
//		x = rl_point;
//		printf("%d\n", x);
		line = readline("minishell> ");
		signal(SIGINT, SIG_IGN);
		if (line == NULL)// || strlen(line) == 0)
		{
			// free(line);
//			free_envlist(g_environ);
//			num = ft_itoa(x + 10);
//			tmp = ft_strjoin("\e[1A\e[", num);
//			str = ft_strjoin(tmp, "Cexit\n");
//			printf("%s", str);
//			x = get_print_start();
//			printf("%d",rl_point);
//			printf("exit\n");
//			rl_line_buffer = "minishell> exit";
//			rl_replace_line("minishell> exit", 0);
//			rl_redisplay();

			if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1)
			{
		//		printf("terminal_width  =%d\n", ws.ws_col);
		//		printf("terminal_height =%d\n", ws.ws_row);
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
//			else
//				printf("\e[1A\e[%dCexit\n", x + 9);
//			if (x == 1)
//				printf("\e[1A\e[11Cexit\n");
//			else
//				printf("\e[1A\e[13Cexit\n");

//			printf("\e[6n");
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
