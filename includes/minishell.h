/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:05:43 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/12 17:05:23 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/tokenization/tokenize.h"
# include "../src/memory/memory.h"
# include "../src/scan/scan.h"
# include "../src/utils/utils.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "lib/libft/libft.h"
# include "../src/ast/ast.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>

char	*take_command(void);
char	*search_path(char *cmnd, char *envp[]);
int		process_command(const char *cmnd, char *envp[]);
t_token	*here_doc(t_token *tokens);
char	*handle_heredoc(char *delim);
t_ast	*parse(t_token **tokens);

void	print_ast_recursive(t_ast *node, int level, int is_last);
char	*ft_strcat(char *dest, const char *src);
char	*ft_realloc(char *ptr, size_t size);
char	*ft_strcpy(char *dst, const char *src);
void	print_indent(int level, int is_last);
void	print_command_node(t_ast *node, int level, int is_last);
void	print_pipeline_node(t_ast *node, int level, int is_last);
void	print_logical_node(t_ast *node, int level, int is_last);
void	print_subshell_node(t_ast *node, int level, int is_last);
void	print_ast(t_ast *root);

#endif
