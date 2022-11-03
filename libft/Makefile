CC	:=	gcc
CFLAGS	:=	-Wall -Wextra -Werror -c
NAME	:=	libft.a
INCLUDE = -I includes
SRCS_DIR = srcs/
SRCS_FILES	=	ft_strlen.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_strrchr.c \
		ft_strnstr.c \
		ft_strncmp.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memmove.c \
		ft_atoi.c \
		ft_atol.c \
		ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_calloc.c \
		ft_strchr.c \
		ft_strdup.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_split.c \
		ft_itoa.c \
		ft_ltoa.c \
		ft_utoa.c \
		ft_ultoa.c \
		ft_ultoxtoa.c \
		ft_strmapi.c \
		ft_striteri.c\
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_searchidx.c \
		ft_hextoi.c \
		ft_abs.c \
		ft_max.c \
		ft_joinfree.c \
		ft_atol_bool.c
X_SRCS_DIR = xsrcs/
X_SRCS_FILES = error_checker.c \
	ft_xstrdup.c \
	xclose.c \
	xdup.c \
	xfork.c \
	xopen.c \
	xpipe.c \
	ft_xstrdup.c \
	xunlink.c

B_SRCS_DIR = list/
B_SRCS_FILES	=	ft_lstnew.c \
	 		ft_lstadd_front.c \
	 		ft_lstsize.c \
	 		ft_lstlast.c \
	 		ft_lstadd_back.c \
	 		ft_lstdelone.c \
	 		ft_lstclear.c \
	 		ft_lstiter.c \
	 		ft_lstmap.c
GET_NEXT_LINE_DIR = get_next_line/
GET_NEXT_LINE_FILES = get_next_line.c
#				get_next_line_utils.c
b_flag	=	false
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))
B_SRCS = $(addprefix $(B_SRCS_DIR), $(B_SRCS_FILES))
X_SRCS = $(addprefix $(X_SRCS_DIR), $(X_SRCS_FILES))
GNL_SRCS = $(addprefix $(GET_NEXT_LINE_DIR), $(GET_NEXT_LINE_FILES))
OBJS	:=	$(SRCS:.c=.o)
OBJS	+=	$(X_SRCS:.c=.o)
B_OBJS	:=	$(B_SRCS:.c=.o)
GNL_OBJS := $(GNL_SRCS:.c=.o)
OBJS += $(GNL_OBJS)
ifdef WITH_BONUS
OBJS += $(B_OBJS)
endif

all: $(NAME)

bonus:
	make WITH_BONUS=1

$(NAME): $(OBJS)
	ar rc $@ $^

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -f $(OBJS) $(B_OBJS)

fclean:	clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
