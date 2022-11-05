CC = gcc
NAME = minishell

SRCS = main.c \
		minishell.c

ENV_DIR = srcs/env/
ENV_FILES = env.c \
	env_utils.c \
	search_env.c
ENV_SRCS = $(addprefix $(ENV_DIR), $(ENV_FILES))

UTILS_DIR = srcs/utils/
UTILS_FILES = err_exit.c \
	exit_output_control.c \
	free_utils.c \
	utils.c
UTILS_SRCS = $(addprefix $(UTILS_DIR), $(UTILS_FILES))

PARCER_DIR = srcs/parser/
PARCER_FILES = parse.c \
	parse_utils.c \
	parse_heredoc_utils.c \
	parse_heredoc.c \
	parse_redirect.c \
	parse_signal.c \
	syntax_error.c
PARCER_SRCS = $(addprefix $(PARCER_DIR), $(PARCER_FILES))

EXPAND_DIR = srcs/expander/
EXPAND_FILES = expansion_dollar.c \
	expansion.c \
	expansion_heredoc.c \
	expansion_normal.c \
	expansion_quote.c \
	expansion_redirect.c \
	expansion_utils.c
EXPAND_SRCS = $(addprefix $(EXPAND_DIR), $(EXPAND_FILES))

BUILTIN_DIR = srcs/builtins/
BUILTIN_FILES = ft_cd.c \
	cd_error.c \
	ft_echo.c \
	ft_env.c \
	ft_exit.c \
	ft_export.c \
	export_error.c \
	export_utils.c \
	ft_pwd.c \
	ft_unset.c \
	print_prefix_env.c
BUILTIN_SRCS = $(addprefix $(BUILTIN_DIR), $(BUILTIN_FILES))

EXEC_DIR = srcs/execution/
EXEC_FILES = execute.c \
	connect_redirects.c \
	exec_cmd.c \
	exec_error.c \
	exec_others.c \
	exec_utils.c \
	handle_path.c
EXEC_SRCS = $(addprefix $(EXEC_DIR), $(EXEC_FILES))

LEXER_DIR = srcs/lexer/
LEXER_FILES = lexer.c \
	lexer_utils.c \
	lexer_error.c
LEXER_SRCS = $(addprefix $(LEXER_DIR), $(LEXER_FILES))

OBJS = $(SRCS:.c=.o)
PARCER_OBJS = $(PARCER_SRCS:.c=.o)
EXPAND_OBJS = $(EXPAND_SRCS:.c=.o)
LEXER_OBJS = $(LEXER_SRCS:.c=.o)
BUILTIN_OBJS = $(BUILTIN_SRCS:.c=.o)
UTILS_OBJS = $(UTILS_SRCS:.c=.o)
EXEC_OBJS = $(EXEC_SRCS:.c=.o)
ENV_OBJS = $(ENV_SRCS:.c=.o)
OBJS += $(PARCER_OBJS)
OBJS += $(EXPAND_OBJS)
OBJS += $(LEXER_OBJS)
OBJS += $(BUILTIN_OBJS)
OBJS += $(EXEC_OBJS)
OBJS += $(UTILS_OBJS)
OBJS += $(ENV_OBJS)
CFLAGS = -g -Werror -Wextra -Wall -I $(shell brew --prefix readline)/include
LDFLAGS = -L$(shell brew --prefix readline)/lib -lreadline -lhistory
INCLUDE = -I includes -I ./libft/includes 
LIBFT = libft/libft.a

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)

$(LIBFT):
	$(MAKE) -C ./libft

clean:
	$(MAKE) clean -C ./libft
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) fclean -C ./libft
	$(RM) $(NAME) test

re: fclean all


.PHONY: all clean fclean re
