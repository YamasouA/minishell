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
	//ND_REDIRECT_IN,
	//ND_REDIRECT_OUT,
	//ND_HEREDOC,
	//ND_APPEND,
	//ND_NULL,
	//ND_ASSIGN,
	//ND_VAR,
	ND_COMMAND,
	//ND_ARG,
};

typedef e_redirect_type t_redirect_type;
enum e_redirect_type
{
	REDIRECT_IN 1,
	REDIRECT_OUT 2,
	HEREDOC 4,
	APPEND 8,
}

typedef s_redirect	t_redirect;
struct s_redirect
{
	char	*delemiter;
	t_redirect_type	type;
	char	*file_name;
};

typedef struct s_cmd	t_cmd;
struct s_cmd
{
	//int	flag;
	char	**cmd;
	t_redirect	*redirect_in;
	t_redirect	*redirect_out;
	//char	*delemiter;
	//char	**redirect_in;
	//char	**redirect_out; // >
	//char	**redirect_append; // >>
};

typedef struct s_node t_node;
struct s_node
{
	t_node_kind	kind;
	t_node		*lhs;
	t_node		*rhs;
	//char		**cmd;
	t_cmd		*cmd;
};


void minishell(int argc, char **argv);
char *read_line();
t_node *parse(t_token *tok);

// lexer.c
t_token *lexer(char *line);

// lexer_utils.c
void ft_exit(char *msg);
