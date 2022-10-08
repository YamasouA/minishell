#include "minishell.h"

t_env	*g_environ;

static void tester(void)
{
	int fd;
	char *file_name;
	char *line;
	t_token	*tok;
	t_node	*node;
//	t_env	*environ;
	
	g_environ = create_env();
	file_name = "test.txt";
//	file_name = "test3.txt";
	fd = open(file_name, O_RDONLY);
	line = NULL;
	do {
		printf("\n=========================\n");
		free(line);
		line = get_next_line(fd);
		if (line[0] == '/' && line[1] == '/')
			continue;
		if (line == NULL)
			break;
		printf("input line: %s\n", line);
		line[ft_strlen(line) - 1] = '\0';
		tok = lexer(line);
		node = parse(tok);
		node = expansion(node);
		printf("==EXPANSION==\n");
		print_node(node, 0);
	} while (1);
	free(line);
}

int main() {
	tester();
}
