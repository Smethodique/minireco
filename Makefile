NAME = minishell
SRCS =  main.c  parsing/parser.c herdoc/util_herdoc.c  \
parsing/parse_token.c parsing/parse_token_norme.c parsing/toknaize.c parsing/quote_helper.c \
parsing/handle_com_arg.c parsing/lexer.c parsing/handle_env.c parsing/expand.c \
parsing/quote_helperT.c  parsing/helplexer.c execution/redirect_utils.c execution/exec.c \
execution/redirect.c builtins/echo.c builtins/cd.c builtins/pwd.c builtins/export.c \
builtins/builtins.c builtins/env.c execution/signals.c execution/handle_pipe.c      \
herdoc/expande_her.c herdoc/herdoc_one.c


OBJS = $(SRCS:.c=.o)
LIBFT = 1337Libft/libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -L1337Libft -lft -lreadline

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C 1337Libft

clean:
	rm -f $(OBJS)
	make -C 1337Libft clean

fclean: clean
	rm -f $(NAME)
	make -C 1337Libft fclean

re: fclean all

.PHONY: all clean fclean re