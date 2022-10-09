#include "minishell.h"

t_env *g_environ;

void minishell(int argc, char **argv)
{
	char *line;
	t_token	*tok;
	t_node	*node;

	while (1)
	{
		line = readline("> ");
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
