NAME = libftprintf.a
SRC = ft_printf.c print_uint.c print_int.c\
      print_str.c print_ptr.c print_hex_low.c\
      print_hex_upp.c uintptr_to_hex.c print_char.c
OBJ = $(SRC:.c=.o)
HEADER = libftprintf.h

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c $(HEADER)
	cc -Wall -Wextra -Werror -fPIE -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
