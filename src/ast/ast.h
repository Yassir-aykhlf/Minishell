/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:36:41 by arajma            #+#    #+#             */
/*   Updated: 2025/03/08 04:52:58 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "../../includes/minishell.h"

// main function to parse input

t_ast	*parse_subshell(t_token **tokens);
t_ast	*parse_command(t_token **tokens);
t_ast	*parse_pipeline(t_token **tokens);
t_ast	*parse_logical_expr(t_token **tokens);
t_ast	*parse(t_token **tokens);

// init and some utils

t_ast	*parse_factor(t_token **tokens);
t_ast	*create_logical_node(t_logical_op op, t_ast *left, t_ast *right);
t_ast	*create_pipeline_node(void);
t_ast	*create_command_node(void);
t_ast	*create_subshell_node(void);

// ast utils functions

void	add_redirect(t_ast *cmd_node, t_token_type type, char *file);
void	add_argument(t_ast *cmd_node, char *arg);
void	add_command_to_pipeline(t_ast *pipeline_node, t_ast *cmd_node);
int		is_redirect(t_token *token);
int		is_cmd_finished(t_token *token);

#endif