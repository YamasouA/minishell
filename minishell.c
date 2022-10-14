#include "minishell.h"

t_env *g_environ;

static void	signal_handler(int sig)
{
	printf("%d\n", sig);
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		
	}
	else if (sig == SIGQUIT)
	{
	}
}

void minishell(int argc, char **argv)
{
	char *line;
	t_token	*tok;
	t_node	*node;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		
		line = readline("minishell> ");
		if (line == NULL || strlen(line) == 0)
		{
			// free(line);
			continue;
		}
		g_environ = create_env();
		add_history(line);
//		printf("line: %s\n", line);
		tok = lexer(line);
		node = parse(tok);
		node = expansion(node);
		//printf("==EXPANSION==\n");
		//print_node(node, 0);
		exec(node, 0);
		free(line);
	}
	argc = 0;
	strlen(*argv);
}
