/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:05:43 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/05 12:26:48 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/tokenization/tokenize.h"
# include "../src/lexer/lexer.h"
# include "../src/utils/utils.h"
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

typedef enum e_node_type
{
	SIMPLE_COMMAND,
	PIPELINE,
	AND_NODE,
	OR_NODE,
	COMPOUND_COMMAND,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	HERE_DOC
}	t_node_type;

typedef enum e_op
{
	OR,
	AND
}	t_op;

typedef enum t_ast_type
{
	SIMPLE_COMMAND,
	PIPELINE,
	LOG_OP,
	REDIRECTIONS
}	t_ast_type;

typedef struct s_redir
{
	int		type;
	char	*file;
}	t_redir;

typedef struct s_ast	t_ast;

typedef struct s_ast
{
	t_ast_type type;
	union
	{
		struct
		{
			t_op ;
			t_ast *left;
			t_ast *right;
		} op;
		struct
		{
			t_ast **childs;
			int	counter;
		} pipline;
		struct
		{
			t_ast *cmd;
			t_redir		*redirects;
			size_t		redirect_count;
		} cmd;
	};
}	t_ast;

#endif
