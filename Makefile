NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = src/main.c
OBJS = $(SRCS:.c=.o)

RM = rm -f

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re

