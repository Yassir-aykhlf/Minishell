/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:32:17 by arajma            #+#    #+#             */
/*   Updated: 2025/05/03 14:57:59 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*create_subshell_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_SUBSHELL;
	node->u_data.s_subshell.command = NULL;
	return (node);
}

t_ast	*create_command_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_COMMAND;
	node->u_data.s_cmd.argv = NULL;
	node->u_data.s_cmd.redirects = NULL;
	return (node);
}

t_ast	*create_pipeline_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_PIPELINE;
	node->u_data.s_pipeline.commands = NULL;
	node->u_data.s_pipeline.count = 0;
	return (node);
}

t_ast	*create_logical_node(t_logical_op op, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_LOGICAL;
	node->u_data.s_op.operat = op;
	node->u_data.s_op.left = left;
	node->u_data.s_op.right = right;
	return (node);
}
