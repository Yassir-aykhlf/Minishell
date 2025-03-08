/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:05:43 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/08 15:27:01 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/tokenization/tokenize.h"
# include "../src/lexer/lexer.h"
# include "../src/ast/ast.h"
# include "../src/utils/utils.h"
# include "../src/memory/memory.h"
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

char	*take_command(void);
char	*search_path(char *cmnd, char *envp[]);
int		process_command(const char *cmnd, char *envp[]);

typedef enum e_logical_op
{
	LOGICAL_AND,
	LOGICAL_OR
}	t_logical_op;

typedef enum e_ast_type {
	NODE_LOGICAL,
	NODE_PIPELINE,
	NODE_COMMAND,
	NODE_SUBSHELL
}	t_ast_type;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	bool			append; //for >> redirection
}	t_redir;

typedef struct	s_ast t_ast;

typedef struct	s_ast
{
	t_ast_type type;
	union
	{
		struct
		{
			t_logical_op	operat;
			t_ast			*left;
			t_ast			*right;
		}		op;
		struct
		{
			t_ast	**commands;
			int		count;
		}		pipeline;
		struct
		{
			char	**cmd;
			t_redir	*redirects;
			size_t	redirect_count;
		}		cmd;
		struct
		{
			t_ast	*command;
		}		subshell;
	}	data;
}	t_ast;


#endif
