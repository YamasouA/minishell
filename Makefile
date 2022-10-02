CC = gcc
# CFLAGS = -Wall -Wextra -Werror -I $(shell brew --prefix readline)/include -g -lreadline -lhistory -L$(shell brew --prefix readline)/lib
NAME = test

SRCS = main.c \
		minishell.c \
		get_next_line.c \
		get_next_line_utils.c

PARCER_DIR = srcs/parser/
PARCER_FILES = parse.c \
	parse_utils.c
PARCER_SRCS = $(addprefix $(PARCER_DIR), $(PARCER_FILES))

EXPAND_DIR = srcs/expander/
EXPAND_FILES = expansion.c
EXPAND_SRCS = $(addprefix $(EXPAND_DIR), $(EXPAND_FILES))

LEXER_DIR = srcs/lexer/
LEXER_FILES = lexer.c \
	lexer_utils.c
LEXER_SRCS = $(addprefix $(LEXER_DIR), $(LEXER_FILES))

LEXER_TEST_SRCS = srcs/lexer/tester.c \
	get_next_line.c \
	get_next_line_utils.c \
	srcs/lexer/lexer.c \
	srcs/lexer/lexer_utils.c \
	srcs/parser/parse.c \
	srcs/parser/parse_utils.c
LEXER_TEST_OBJS = $(LEXER_TEST_SRCS:.c=.o)

OBJS = $(SRCS:.c=.o)
PARCER_OBJS = $(PARCER_SRCS:.c=.o)
EXPAND_OBJS = $(EXPAND_SRCS:.c=.o)
LEXER_OBJS = $(LEXER_SRCS:.c=.o)
OBJS += $(PARCER_OBJS)
OBJS += $(EXPAND_OBJS)
OBJS += $(LEXER_OBJS)
CFLAGS = -g -Werror -Wextra -Wall -I $(shell brew --prefix readline)/include
LDFLAGS = -lreadline -lhistory -L$(shell brew --prefix readline)/lib
INCLUDE = -I includes -I ./libft/includes 
LIBFT = libft/libft.a

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(NAME) $(OBJS) $(LIBFT)

ltest: $(LEXER_TEST_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o test $(LEXER_TEST_OBJS) $(LIBFT)

$(LIBFT):
	$(MAKE) -C ./libft

clean:
	$(MAKE) clean -C ./libft
	$(RM) *.o

fclean: clean
	$(MAKE) fclean -C ./libft
	$(RM) $(NAME) test

re: fclean all


.PHONY: all clean fclean re
