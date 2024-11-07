GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
YELLOW = \033[0;33m
RESET = \033[0m

NAME = minishell
SRCS = main.c parsing/parser.c herdoc/util_herdoc.c \
parsing/parse_token.c parsing/parse_token_norme.c parsing/toknaize.c parsing/quote_helper.c \
parsing/handle_com_arg.c parsing/lexer.c parsing/handle_env.c parsing/expand.c \
parsing/quote_helperT.c parsing/helplexer.c execution/redirect_utils.c execution/exec.c \
execution/redirect.c builtins/echo.c builtins/cd.c builtins/pwd.c builtins/export.c \
builtins/builtins.c builtins/env.c execution/signals.c execution/handle_pipe.c \
herdoc/expande_her.c herdoc/herdoc_one.c builtins/helper_cd.c parsing/parser_help.c \
parsing/parser_add.c parsing/parser_utils.c execution/handle_pipe_helper.c \
execution/exec_helper.c builtins/unset.c builtins/builtins_utils.c builtins/export_helper.c \
builtins/append_export.c builtins/append_helper.c execution/path_expantion.c
OBJS = $(SRCS:.c=.o)
LIBFT = 1337Libft/libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror  #-g    -fsanitize=address  -I/Users/$(USER)/homebrew/opt/readline/include
LDFLAGS = -L1337Libft -lft -L/Users/$(USER)/homebrew/opt/readline/lib -lreadline

# Add silent flags
MAKEFLAGS += --silent


all: $(NAME)
                                                                        
HEADER = \
	"\033[0;32m]" \
	"@@@@@@   @@@  @@@     @@@@@@   @@@  @@@  @@@@@@@@  @@@       @@@  \n" \
	"@@@@@@@   @@@@ @@@    @@@@@@@   @@@  @@@  @@@@@@@@  @@@       @@@  \n" \
	"!@@       @@!@!@@@    !@@       @@!  @@@  @@!       @@!       @@!  \n" \
	"!@!       !@!!@!@!    !@!       !@!  @!@  !@!       !@!       !@!  \n" \
	"!!@@!!    @!@ !!@!    !!@@!!    @!@!@!@!  @!!!:!    @!!       @!!  \n" \
	" !!@!!!   !@!  !!!     !!@!!!   !!!@!!!!  !!!!!:    !!!       !!!  \n" \
	"     !:!  !!:  !!!         !:!  !!:  !!!  !!:       !!:       !!:  \n" \
	"    !:!   :!:  !:!        !:!   :!:  !:!  :!:        :!:       :!:  \n" \
	":::: ::    ::   ::    :::: ::   ::   :::   :: ::::   :: ::::   :: ::::  \n" \
	":: : :    ::    :     :: : :     :   : :  : :: ::   : :: : :  : :: : :  \n" \
	"\033[0m" \ 
    
$(NAME): $(OBJS) $(LIBFT)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)Build complete!$(RESET)"
	@echo "$(GREEN)✓$(RESET) $(YELLOW)Ready to execute: ./$(NAME)$(RESET)"
	@echo "$(GREEN)"
	@echo $(HEADER)
	@echo "$(RESET)"

%.o: %.c
	@printf "$(BLUE)Compiling %-33.33s\r$(RESET)" $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@echo "$(BLUE)Building libft...$(RESET)"
	@make -C 1337Libft
	@echo "$(GREEN)Libft complete!$(RESET)"

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@make -C 1337Libft clean
	@echo "$(GREEN)Clean complete!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Cleaning executable...$(RESET)"
	@rm -f $(NAME)
	@make -C 1337Libft fclean
	@echo "$(GREEN)Full clean complete!$(RESET)"

re: fclean all

.Secondary:
.PHONY: all clean fclean re