#include "minishell.h"

void minishell(int argc, char **argv)
{
    char *line;

    while (1)
    {
        line = readline("> ");
        if (line == NULL || strlen(line) == 0)
        {
            // free(line);
            break;
        }
        add_history(line);
        printf("line: %s\n", line);
        lexer(line);
        // parse();
        // exec();
        free(line);
    }
    argc = 0;
    strlen(*argv);
}