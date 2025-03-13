/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:50:42 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/13 16:59:54 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "../../src/ast/ast.h"
# include <fcntl.h>
# include <sys/stat.h>

# define EMPTY_AST 1

int	ft_execute(t_ast *ast, char *envp[]);

#endif
