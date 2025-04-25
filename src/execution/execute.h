/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:50:42 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/25 21:34:48 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "../../src/ast/ast.h"
# include <fcntl.h>
# include <sys/stat.h>

# define EMPTY_AST 1

bool	is_valid_var_char(char c);
int		*get_exit_status(void);
void	set_exit_status(int status);
int		ft_execute(t_ast *root);
int		execute_command(t_ast *cmd);
int		execute_pipeline(t_ast *node);
int		execute_recursive(t_ast *node);
int		redirect(t_redir *redirects, size_t count);
char	*search_path(char *cmd);

int		execute_child_process(t_ast *node, int i, int prev_pipe_read, int pipe_fds[2]);
int		handle_parent_process(int i, int *prev_pipe_read, int pipe_fds[2], t_ast *node);
int		validate_pipeline(t_ast *node);

#endif
