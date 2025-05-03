/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:50:39 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 18:55:35 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_recursive(t_ast *node)
{
	int	status;

	if (!node)
		return (EMPTY_AST);
	status = 0;
	if (node->type == NODE_COMMAND)
		status = execute_command(node);
	else if (node->type == NODE_PIPELINE)
		status = execute_pipeline(node);
	else if (node->type == NODE_LOGICAL)
		status = execute_logical(node);
	else if (node->type == NODE_SUBSHELL)
		status = execute_subshell(node);
	else
		status = spit_error(EXIT_FAILURE, "Unknown AST node type", false);
	return (status);
}

int	ft_execute(t_ast *root)
{
	if (!root)
		return (EMPTY_AST);
	return (execute_recursive(root));
}
