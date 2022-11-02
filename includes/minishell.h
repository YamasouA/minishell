#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "get_next_line.h"

# define PROMPT_LENGTH 11
# define TMPFILE "tmp/tempfile"
# define EXPORT_NONE 0
# define EXPORT_NEW 1
# define EXPORT_APPEND 2
# define EXPORT_ERROR 4

typedef struct termios			t_termios;
typedef enum e_token_kind		t_kind;
enum e_token_kind
{
	TK_KEYWORD,
	TK_STR,
	TK_IDENT,
	TK_HEAD,
};

typedef struct s_token			t_token;
struct s_token
{
	t_kind		kind;
	t_token		*next;
	char		*str;
	size_t		len;
};

typedef struct s_env			t_env;
struct s_env
{
	bool		is_env;
	char		*key;
	char		*value;
	t_env		*next;
};

typedef struct s_sh_var			t_sh_var;
struct s_sh_var
{
	t_env			*environ;
	int				exit_status;
	volatile bool	signal;
};

extern t_sh_var					g_sh_var;

typedef enum e_node_kind		t_node_kind;
enum e_node_kind
{
	ND_PIPE,
	ND_COMMAND,
};

typedef enum e_redirect_type	t_redirect_type;
enum e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND,
	REDIRECT_NONE,
};

typedef struct s_redirect		t_redirect;
struct s_redirect
{
	char			*delemiter;
	t_redirect_type	type;
	char			*file_name;
	t_redirect		*next;
	char			*documents;
};

typedef struct s_cmd			t_cmd;
struct s_cmd
{
	char		**cmd;
	t_redirect	*redirect_in;
	t_redirect	*redirect_out;
};

typedef struct s_node			t_node;
struct s_node
{
	t_node_kind	kind;
	t_node		*lhs;
	t_node		*rhs;
	t_cmd		*cmd;
};

t_env	*create_env(void);
char	*search_key(t_env *env, char *key);
t_env	*search_env(t_env *env, char *key);
void	set_data(char *environ, t_env *new);
void	add_env(t_env **env, t_env *new);
t_env	*env_last(t_env *env);
void	update_or_add_value(t_env **env, char *key, char *value);
void	print_env(t_env *env);

// minishell.c
void	minishell(void);
void	set_signal_handler(int signum, sig_t sighandler);
//char	*read_line(void);

// srcs/expander
t_node	*expansion(t_node *node);
char	*expand(char *str, bool heredoc);

// srcs/parser
t_node	*parse(t_token **tok, bool *heredoc_err);
void	print_node(t_node *node, int tab_n);
void	free_node(t_node *node);
void	free_token(t_token *tok);

// srcs/lexer
t_token	*lexer(char *line);
bool	ft_isspace(char str);

// lexer_utils.c
void	ft_exit2(char *msg);

// builtins
char	*join_with_connector(char *s1, char *s2, char connector);
int		ft_echo(char **strs);//, t_env *envp);
int		ft_cd(char **strs);//, t_env *env);
int		ft_pwd(char **strs);//,t_env *envp);
int		ft_export(char **args);//, t_env *envp);
int		ft_unset(char **keys);//, t_env *envp);
int		ft_env(char **strs);//, t_env *envp);
int		ft_exit(char **strs);//, t_env *envp);

// srcs/execution
void	exec(t_node *node, int pipe_flag);

// srcs/utils
void	err_exit(char *msg);
#endif
