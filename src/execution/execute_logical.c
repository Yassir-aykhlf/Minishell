/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logical.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:51:15 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 17:52:20 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int execute_logical(t_ast *node)
{
	int	left_status;
	int	right_status;

	if (!node || node->type != NODE_LOGICAL || !node->u_data.s_op.left)
		return (spit_error(EXIT_FAILURE, "Invalid logical node", false));
	left_status = execute_recursive(node->u_data.s_op.left);
	if (node->u_data.s_op.operat == LOGICAL_AND)
	{
		if (left_status == 0 && node->u_data.s_op.right)
			right_status = execute_recursive(node->u_data.s_op.right);
		else
			right_status = left_status;
	}
	else if (node->u_data.s_op.operat == LOGICAL_OR)
	{
		if (left_status != 0 && node->u_data.s_op.right)
			right_status = execute_recursive(node->u_data.s_op.right);
		else
			right_status = left_status;
	}
	else
		return (spit_error(EXIT_FAILURE, "Unknown logical operator", false));
	return (right_status);
}
