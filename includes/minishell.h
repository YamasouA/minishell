#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
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

typedef struct s_env t_env;
struct s_env
{
	bool	is_env;
	char	*key;
	char	*value;
	t_env	*next;
};

extern t_env *g_environ;


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

typedef enum e_redirect_type t_redirect_type;
enum e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND,
};

typedef struct s_redirect	t_redirect;
struct s_redirect
{
	char			*delemiter;
	t_redirect_type	type;
	char			*file_name;
	t_redirect		*next;
	char			*documents;
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

//extern	char **environ;

t_env	*create_env();
char	*search_key(t_env *env, char *key);
t_env	*search_env(t_env *env, char *key);
void	set_data(char *environ, t_env *new);
void	add_env(t_env **env, t_env *new);
t_env	*env_last(t_env *env);
void	update_or_add_value(t_env *env, char *key, char *value);
void	print_env(t_env *env);

void minishell(int argc, char **argv);
char *read_line();

// srcs/expander
t_node	*expansion(t_node *node);
char	*expand(char *str, bool here_doc);

// srcs/parser
t_node *parse(t_token *tok);
void	print_node(t_node *node, int tab_n);

// srcs/lexer
t_token *lexer(char *line);
bool	ft_isspace(char *str);

// lexer_utils.c
void ft_exit2(char *msg);

// builtins
char	*join_with_connector(char *s1, char *s2, char connector);
//char	*join_slash(char *s1, char *s2);
int	ft_echo(char **strs);//, t_env *envp);
int	ft_cd(char **strs);//, t_env *env);
int	ft_pwd(char **strs);//,t_env *envp);
int	ft_export(char **args);//, t_env *envp);
int	ft_unset(char **keys);//, t_env *envp);
int	ft_env(char **strs);//, t_env *envp);
int	ft_exit(char **strs);//, t_env *envp);

void	exec(t_node *node, int pipe_flag);
