/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:05:43 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/03 17:57:09 by yaykhlf          ###   ########.fr       */
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

bool	is_whitespace(char c);

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SINGLE_QUOTED,
	TOKEN_DOUBLE_QUOTED,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum e_node_type
{
	SIMPLE_COMMAND,    /* Basic command with arguments */
	PIPELINE,          /* Pipe between commands */
	AND_NODE,          /* Logical AND (&&) */
	OR_NODE,           /* Logical OR (||) */
	COMPOUND_COMMAND,  /* Compound command (a group) */
	REDIRECT_IN,       /* Input redirection (<) */
	REDIRECT_OUT,      /* Output redirection (>) */
	REDIRECT_APPEND,   /* Output append redirection (>>) */
	HERE_DOC           /* Heredoc (<<) */
}	t_node_type;

typedef struct s_redir
{
	int		type;           /* Type of redirection */
	char	*file;         /* File name */
}	t_redir;

typedef struct s_ast
{
	t_node_type type;
	union
	{
		/* For command nodes */
		struct
		{
			char *cmd;
			char **args;
		} binary_node;
		
		/* For nodes with multiple children */
		struct
		{
			t_ast **childs;
			t_redir **redirection;
		} unary_node;
	};
}	t_ast;

#endif
