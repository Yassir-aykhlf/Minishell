/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:32:17 by arajma            #+#    #+#             */
/*   Updated: 2025/03/08 16:20:41 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Helper function to create a new subshell node */
t_ast	*create_subshell_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_SUBSHELL;
	node->data.subshell.command = NULL;
	return (node);
}

/* Helper function to create a new command node */
t_ast	*create_command_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_COMMAND;
	node->data.cmd.cmd = NULL;
	node->data.cmd.redirects = NULL;
	node->data.cmd.redirect_count = 0;
	return (node);
}

/* Helper function to create a new pipeline node */
t_ast	*create_pipeline_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_PIPELINE;
	node->data.pipeline.commands = NULL;
	node->data.pipeline.count = 0;
	return (node);
}

/* Helper function to create a new logical node */
t_ast	*create_logical_node(t_logical_op op, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_LOGICAL;
	node->data.op.operat = op;
	node->data.op.left = left;
	node->data.op.right = right;
	return (node);
}

/* Function to handle subshells */
t_ast	*parse_factor(t_token **tokens)
{
	if (*tokens && (*tokens)->type == TOKEN_PAREN_OPEN)
		return (parse_subshell(tokens));
	else
		return (parse_command(tokens));
}
