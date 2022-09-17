#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

char *read_line()
{
    char *line = NULL;

    while (1)
    {
        line = readline("> ");
        if (line == NULL || strlen(line) == 0)
        {
            // free(line);
            break;
        }
        add_history(line);
        // parse();
        // exec();
        // free(line);
    }
    return (line);
}
