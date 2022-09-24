#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include "get_next_line.h"

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
	t_kind	kind;
	t_token	*next;
	char	*str;
	size_t	len;
};

typedef enum e_node_kind t_node_kind;
enum e_node_kind
{
	ND_PIPE,
	ND_REDIRECT_IN,
	ND_REDIRECT_OUT,
	ND_HEREDOC,
	ND_APPEND,
	ND_NULL,
	ND_ASSIGN,
	ND_VAR,
	ND_COMMAND,
	ND_ARG,
};

typedef struct s_node t_node;
struct s_node
{
	t_node_kind	kind;
	t_node		*lhs;
	t_node		*rhs;
	char		*str;
};


void minishell(int argc, char **argv);
char *read_line();
void parse(void);

// lexer.c
void lexer(char *line);

// lexer_utils.c
void ft_exit(char *msg);
