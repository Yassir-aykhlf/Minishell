NAME = minishell
CC = cc
CFLAGS = -g3 -lreadline -Wall -Wextra -Werror

LIBFT_PATH = includes/lib/libft
LIBFT = $(LIBFT_PATH)/libft.a

MAIN_SRC = src/main/shell.c
SCAN_SRC = src/scan/scan.c src/scan/scan_utils.c
AST_SRC = src/ast/ast.c src/ast/ast_init.c src/ast/ast_utils.c src/ast/ast_utils2.c
MEMORY_SRC = src/memory/memory.c
TOKENIZATION_SRC = src/tokenization/tokenize.c src/tokenization/tokenize_init.c \
                   src/tokenization/tokenize_utils.c
UTILS_SRC = src/utils/utils.c src/utils/more_utils.c
HEREDOC_SRC = src/heredoc/heredoc.c src/heredoc/heredoc_utils.c
EXECUTION_SRC = src/execution/builtin_cd.c src/execution/builtin_echo.c \
                src/execution/builtin_exit.c src/execution/builtin_export.c \
                src/execution/builtin_export_helper.c src/execution/builtins.c \
                src/execution/execute.c src/execution/execute_command.c \
                src/execution/execute_command_helpers.c src/execution/execute_command_utils.c \
                src/execution/execute_command_utils_utils.c src/execution/execute_logical.c \
                src/execution/execute_pipeline.c src/execution/execute_subshell.c \
                src/execution/execution_utils.c src/execution/exit_status.c \
                src/execution/redirection.c
ENVIRONMENT_SRC = src/environment/env.c src/environment/env_utils.c
EXPANSION_SRC = src/expansion/expansion.c src/expansion/expand_utils.c \
                src/expansion/expand_utils2.c src/expansion/expand_utils3.c
SIGNAL_SRC = src/signal_handeling/singnal_handeling.c

SRC = $(MAIN_SRC) $(SCAN_SRC) $(AST_SRC) $(MEMORY_SRC) $(TOKENIZATION_SRC) \
      $(UTILS_SRC) $(HEREDOC_SRC) $(EXECUTION_SRC) $(ENVIRONMENT_SRC) \
      $(EXPANSION_SRC) $(SIGNAL_SRC)

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
