/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:05:43 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/12 20:30:11 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lib/libft/libft.h"
# include "../src/tokenization/tokenize.h"
# include "../src/ast/ast.h"
# include "../src/expansion/expansion.h"
# include "../src/execution/execute.h"
# include "../src/heredoc/heredoc.h"
# include "../src/environment/env.h"
# include "../src/memory/memory.h"
# include "../src/utils/utils.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "../src/scan/scan.h"
# include "../src/signal_handeling/signal_handeling.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/stat.h>

# define CMD_NOT_FOUND 127
# define SIGINT_EXIT 130
# define SYNTAX_ERROR 2
# define SUCCESS 0
# define ERROR 1

void	print_env(void);
char	*get_command(void);
char	*search_path(char *cmd);
int		interpreter(const char *cmd);
t_ast	*ft_parse(t_token **tokens);
int		syntax_error(t_ast *ast, t_token *tokens);
void	syntax_print(char *token);
bool	is_valid_var_char(char c);
char	*last_token(t_token *tokens);

#endif
