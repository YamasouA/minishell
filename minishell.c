#include "minishell.h"
#include <termios.h>

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

	tcgetattr(0, &oldstate);
	state = oldstate;
	state.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &state);
	ft_putstr_fd("\e[6n", 0);
	x = get_x_pos();
	tcsetattr(0, TCSANOW, &oldstate);
	return (x);
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

//	signal(SIGINT, signal_handler);
	g_environ = create_env();
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		heredoc_err = 0;
		signal(SIGINT, signal_handler);
//		ft_putstr_fd("minishell> ", 1);
		x = get_print_start();
		line = readline("minishell> ");
		signal(SIGINT, SIG_IGN);
		if (line == NULL)// || strlen(line) == 0)
		{
			// free(line);
//			printf("exit\n");
			free_envlist(g_environ);
			if (x == 1)
				printf("\e[1A\e[11Cexit\n");
			else
				printf("\e[1A\e[13Cexit\n");

//			printf("\e[6n");
			exit(0);
		}
		if (strlen(line) == 0)
			continue;
		add_history(line);
//		printf("line: %s\n", line);
		tok = lexer(line);
		if (tok == NULL)
			continue ;
		node = parse(tok, &heredoc_err);
//		printf("ok2\n");
		if (g_signal != 0 || node == NULL)
		{
			g_signal = 0;
			continue ;
		}
//		printf("ok3\n");
		node = expansion(node);
//		printf("ok4\n");
		//printf("==EXPANSION==\n");
		//print_node(node, 0);
		exec(node, 0);
//		printf("ok5\n");
		free(line);
		//free(tok);
		//free(node);
		free_node(node);
		free_token(tok);
	}
	argc = 0;
	strlen(*argv);
}
