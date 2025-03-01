/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:05:43 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/01 21:11:08 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lib/printf/libftprintf.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "lib/libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>
# include <stdbool.h>

typedef struct ASTnode
{
	int				type;
	union{
		{
			char		*cmd;
			char		**args;
		} binary_node,
		{
			[] childs
			[] redirection;
		} unary_node;
		
	}

}				t_ast;

#endif
