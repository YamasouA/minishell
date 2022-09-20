#include "minishell.h"

static void tester(void)
{
	int fd;
	char *file_name;
	char *line;

	file_name = "test.txt";
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
		lexer(line);
	} while (1);
	free(line);
}

int main() {
	tester();
}
