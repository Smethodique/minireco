SRCS =	ft_strlen.c ft_strrchr.c ft_atoi.c  ft_isdigit.c \
       	ft_memcmp.c ft_memset.c ft_strlcat.c ft_strncmp.c ft_tolower.c \
       	ft_bzero.c ft_isalpha.c ft_isprint.c ft_memcpy.c ft_strchr.c \
       	ft_strlcpy.c ft_strnstr.c ft_toupper.c ft_substr.c ft_strjoin.c \
	   	ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
	   	ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_isascii.c ft_memmove.c ft_memchr.c  ft_strdup.c \
	   	GNL/get_next_line.c GNL/get_next_line_utils.c ft_printf/ft_calclen.c ft_printf/ft_printf.c ft_printf/ft_putadress.c \
	   	 ft_printf/ft_putchar.c  ft_printf/ft_putdec.c  ft_printf/ft_puthex.c  ft_printf/ft_putstr.c  ft_printf/ft_putunsdec.c \
	   	 ft_printf/ft_putuphex.c ft_strcpy.c

OBJS = ${SRCS:.c=.o}

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror

${NAME}: $(OBJS)
		ar rcs $(NAME) ${OBJS}

$(OBJS): $(SRCS)
		$(CC) $(CFLAGS) -c $(@:.o=.c) -o $@

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re