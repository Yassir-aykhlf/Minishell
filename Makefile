NAME = minishell
CC = cc
CFLAGS = -g3 -lreadline -Wall -Wextra -Werror

LIBFT_PATH = includes/lib/libft
LIBFT = $(LIBFT_PATH)/libft.a

SRC = $(wildcard src/main/*.c) \
      $(wildcard src/scan/*.c) \
      $(wildcard src/ast/*.c) \
      $(wildcard src/memory/*.c) \
      $(wildcard src/tokenization/*.c) \
      $(wildcard src/utils/*.c) \
      $(wildcard src/heredoc/*.c) \
      $(wildcard src/execution/*.c) \
      $(wildcard src/environment/*.c) \
      $(wildcard src/expansion/*.c) \
      $(wildcard src/signal_handeling/*.c)

INCLUDES = -I includes -I src

all: $(NAME)

$(NAME): $(SRC) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) $(INCLUDES) $(LIBFT)
	@echo "\033[0;32mMinishell compiled successfully\033[0m"

$(LIBFT):
	make -C $(LIBFT_PATH)

clean:
	make -C $(LIBFT_PATH) clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all
