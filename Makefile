NAME = Enigma
CC = cc
CFLAGS = -lreadline #-Wall -Wextra -Werror
HEADER = includes/minishell.h
SRC = src/main/shell.c src/scan/*.c src/ast/*.c src/memory/*.c src/tokenization/*.c src/utils/*.c src/heredoc/*.c src/execution/execute.c
LIBFT = $(LIBFT_PATH)/libft.a
LIBFT_PATH = includes/lib/libft

all: $(NAME)

$(NAME): $(SRC) $(LIBFT) $(HEADER)
	$(CC) $(SRC) $(CFLAGS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

bonus:

clean:
	rm -f $(SRC:.c=.o)
	make -C $(LIBFT_PATH) clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all
