/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:36:41 by arajma            #+#    #+#             */
/*   Updated: 2025/05/04 18:11:33 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "../../src/tokenization/tokenize.h"
# include <stdbool.h>
# include <stdlib.h>

typedef enum e_logical_op
{
	LOGICAL_AND,
	LOGICAL_OR
}	t_logical_op;

typedef enum e_ast_type
{
	NODE_LOGICAL,
	NODE_PIPELINE,
	NODE_COMMAND,
	NODE_SUBSHELL,
	NODE_EMPTY_COMMAND
}	t_ast_type;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	t_mask			*mask;
	struct s_redir	*next;
}	t_redir;

typedef struct s_args
{
	char			*arg;
	t_mask			*mask;
	struct s_args	*next;
}	t_args;

typedef struct s_ast	t_ast;

typedef struct s_ast
{
	t_ast_type	type;
	union
	{
		struct
		{
			t_logical_op	operat;
			t_ast			*left;
			t_ast			*right;
		}		s_op;
		struct
		{
			t_ast	**commands;
			int		count;
		}		s_pipeline;
		struct
		{
			t_args	*argv;
			t_redir	*redirects;
		}		s_cmd;
		struct
		{
			t_ast	*command;
		}		s_subshell;
	}	u_data;
}	t_ast;

t_ast	*parse_subshell(t_token **tokens);
t_ast	*parse_pipeline(t_token **tokens);
t_ast	*parse_logical_expr(t_token **tokens);
t_ast	*ft_parse(t_token **tokens);
t_ast	*create_logical_node(t_logical_op op, t_ast *left, t_ast *right);
t_ast	*handle_redirections(t_ast *node, t_token **tokens);
t_ast	*create_pipeline_node(void);
t_ast	*create_command_node(void);
t_ast	*create_subshell_node(void);
void	add_redirect(t_ast *cmd_node, t_token_type type, char *file,
			t_mask *mask);
void	add_argument(t_ast *cmd_node, char *arg, t_mask *mask);
void	add_command_to_pipeline(t_ast *pipeline_node, t_ast *cmd_node);
int		is_redirect(t_token *token);
int		is_cmd_finished(t_token *token);

#endif
