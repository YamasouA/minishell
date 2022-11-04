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
	char			*delimiter;
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

// env
// env_utils.c
void			set_data(char *environ, t_env *new);
void			add_env(t_env **env, t_env *new);
t_env			*env_last(t_env *env);
t_env			*env_new(char *key, char *value);
size_t			count_envlist(t_env *env);

// env.c
t_env			*create_env(void);
char			**envlist_to_str(t_env *env);
void			update_or_add_value(t_env **env, char *key, char *value);

// search_env.c
char			*search_key(t_env *env, char *key);
t_env			*search_env(t_env *env, char *key);
char			*find_env(char *var, size_t len);
//void	print_env(t_env *env);

// minishell.c
void			minishell(void);
void			set_signal_handler(int signum, sig_t sighandler);
//char	*read_line(void);

// srcs/expander
// expansion.c
t_node			*expansion(t_node *node);
char			*expand(char *str, bool heredoc);
void			expand_cmd_instance(char **cmd_data, bool here_doc);

//expansion_dollar.c
char			*expand_dollar(char *str, char *expanded, int *i);

//expansion_heredoc.c
char			*expand_documents(char *str);

// expansion_normal.c
char			*handle_normal(char *str, int *i, bool heredoc);

// expansion_quote.c
char			*handle_s_quote(char *str, int *i);
char			*handle_d_quote(char *str, int *i, bool here_doc);

// expansion_redirect.c
void			expand_redir_list(t_node *node);

// expansion_utils.c
bool			is_expand_none(char *str, char *expanded, int i);

// parser
// parse.c
t_node			*parse(t_token **tok, bool *heredoc_err);
void			print_node(t_node *node, int tab_n);
void			free_node(t_node *node);
void			free_token(t_token *tok);

//parse_heredoc.c
void			do_heredoc(t_node *node, bool *heredoc_err, int heredoc_flag);

//parse_heredoc_utils.c
char			*ft_ultoa(unsigned long n);
unsigned long	xorshift(void);

//parse_redirect.c
int				which_redir(t_token *tok);
void			parse_redir(t_token **tok, t_node *node, int type, \
								int *error_flag);

//parse_signal.c
void			heredoc_signal_handler(int sig);
int				check_state(void);
void			processing_on_signal(char *line, char *documents, \
										bool *heredoc_err);

//parse_utils.c
bool			peek(t_token *tok, char *op);
bool			consume(t_token **tok, char *op);
size_t			cmd_len(t_token *tok);

//syntax_error.c
t_node			*syntax_error(t_node *node, t_token **tok, t_token *head);
void			free_node(t_node *node);
void			free_cmd(t_cmd *cmd);
void			free_redirect(t_redirect *redir);
void			free_token(t_token *tok);

// lexer
// lexer.c
t_token			*lexer(char *line);

// lexer_utils.c
//void	ft_exit2(char *msg);
bool			ft_isspace(char str);
bool			is_meta(char c);
size_t			len_keyword(char *c);
char			*find_quote(char *line, char quote); //same strchr?
ssize_t			len_word(char *line);

//lexer_error.c
t_token			*tokenize_error(char token, t_token *head);
t_token			*quote_error(char *token, t_token *head);

// builtins
// export_error.c
void			print_invalid_identifier(char *str);
int				print_error_and_usage(char *arg, char *cmd_name);
// export_utils.c
int				which_update_flag(char **key);
bool			is_valid_var(char *str); // shareable unset func
int				is_append_flag(char **key, char *eq_pos);
// ft_cd.c
int				ft_cd(char **strs);
// ft_echo.c
int				ft_echo(char **strs);//, t_env *envp)
// ft_env.c
char			**envlist_to_str(t_env *env);
int				ft_env(char **strs);//, t_env *envp)
// ft_exit.c
int				ft_exit(char **strs);
// ft_export.c
int				ft_export(char **args);
// ft_pwd.c
int				ft_pwd(char **strs);
// ft_unset.c
int				ft_unset(char **keys);//, t_env *envp)
// print_prefix_env.c
void			print_prefix_env(t_env *envp);

// execution
// execute.c
void			exec(t_node *node, int pipe_flag);

// connect_redirects.c
int				do_redirect(t_cmd *cmd);

// exec_cmd.c
int				exe_process(t_cmd *cmd);
pid_t			exe_cmd(t_cmd *cmd, int pipe_flag);

// exec_error.c
void			print_exec_process_error(char *cmd, char *msg, int status);
void			print_access_err(char *msg);
void			err_is_a_directory(char *filename);
int				print_redirect_err(char *file_name);

// exec_others.c
void			exec_others(t_cmd *cmd);

// exec_utils.c
bool			is_redirect(t_cmd *cmd);
bool			which_builtin(char **cmd);
bool			is_path(char *cmd_name);
bool			is_directory(char *pathname);

// handle_path.c
char			*check_path(char *path);

// utils
// err_exit.c
void			err_exit(char *msg);

// free_utils.c
void			free_strs(char **strs);
void			all_free(char *line, t_token *tok, t_node *node);

// utils.c
char			*join_with_connector(char *s1, char *s2, char connector);
#endif
