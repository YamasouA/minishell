#include "minishell.h"

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

void minishell(int argc, char **argv)
{
	char *line;
	t_token	*tok;
	t_node	*node;
	bool	heredoc_err;
	g_signal = 0;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		heredoc_err = 0;
		line = readline("minishell> ");
		if (line == NULL)// || strlen(line) == 0)
		{
			// free(line);
			printf("\e[1A\e[11Cexit\n");
			exit(0);
		}
		g_environ = create_env();
		add_history(line);
//		printf("line: %s\n", line);
		tok = lexer(line);
		node = parse(tok, &heredoc_err);
		if (g_signal != 0)
		{
			g_signal = 0;
			continue ;
		}
		node = expansion(node);
		//printf("==EXPANSION==\n");
		//print_node(node, 0);
		exec(node, 0);
		free(line);
	}
	argc = 0;
	strlen(*argv);
}
