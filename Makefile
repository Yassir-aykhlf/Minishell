NAME = Enigma
CC = cc
CFLAGS = -Wall -Wextra -Werror -lreadline
SRC = shell.c
LIBFT = $(LIBFT_PATH)/libft.a
LIBFT_PATH = includes/lib/libft

all: $(NAME)

$(NAME): $(SRC) $(LIBFT)
	$(CC) $(SRC) $(CFLAGS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

bonus:

clean:
	rm $(SRC:.c=.o)
	make -C $(LIBFT_PATH) clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all
