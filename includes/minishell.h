#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_kind t_kind;
enum e_token_kind
{
    TK_KEYWORD, // | >> > << < $
    TK_STR,
    TK_IDENT,
    // TK_NUM,
    // TK_EOL,
    TK_HEAD,
};

typedef struct s_token t_token;

struct s_token
{
	t_kind kind;
	t_token *next;
	char	*str;
    size_t     len;
};


void minishell(int argc, char **argv);
char *read_line();
void parse(void);
void lexer(char *line);
