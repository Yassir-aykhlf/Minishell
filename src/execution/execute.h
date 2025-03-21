/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:50:42 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/16 11:27:01 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "../../src/ast/ast.h"
# include <fcntl.h>
# include <sys/stat.h>

# define EMPTY_AST 1

int		ft_execute(t_ast *root, char *envp[]);
int		execute_command(t_ast *cmd, char *envp[]);
int		execute_pipeline(t_ast *node, char *envp[]);
int		execute_recursive(t_ast *node, char *envp[]);
int		redirect(t_redir *redirects, size_t count);
char	*search_path(char *cmd, char *envp[]);

#endif
