/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:04:56 by arajma            #+#    #+#             */
/*   Updated: 2025/03/23 23:22:54 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*parse_subshell(t_token **tokens)
{
	t_ast	*subshell_node;
	t_ast	*command_node;

	*tokens = (*tokens)->next;
	command_node = parse_logical_expr(tokens);
	if (!*tokens || !((*tokens)->type == TOKEN_PAREN_CLOSE))
		return (NULL);
	*tokens = (*tokens)->next;
	subshell_node = create_subshell_node();
	subshell_node->u_data.s_subshell.command = command_node;
	return (subshell_node);
}

t_ast	*parse_simple_command(t_token **tokens)
{
	t_token			*current;
	t_ast			*cmd_node;

	cmd_node = create_command_node();
	while (*tokens)
	{
		current = *tokens;
		if (is_cmd_finished(current) || is_redirect(current))
			break ;
		if (current->type == TOKEN_WORD)
		{
			add_argument(cmd_node, current->value);
			*tokens = current->next;
			continue ;
		}
		return (NULL);
	}
	return (cmd_node);
}

t_ast	*parse_command_with_redirects(t_token **tokens)
{
	t_ast	*node;

	if (*tokens && (*tokens)->type == TOKEN_PAREN_OPEN)
		node = parse_subshell(tokens);
	else
		node = parse_simple_command(tokens);
	if (node && node->type == NODE_COMMAND && 
		!node->u_data.s_cmd.argv[0] && 
		node->u_data.s_cmd.redirect_count == 0)
	{
		return (NULL);
	}
	return (handle_redirections(node, tokens));
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*pipeline_node;
	t_ast	*cmd_node;

	pipeline_node = create_pipeline_node();
	if (!*tokens || is_cmd_finished(*tokens))
		return (NULL);
	cmd_node = parse_command_with_redirects(tokens);
	if (!cmd_node)
		return (NULL);
	add_command_to_pipeline(pipeline_node, cmd_node);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		if (!*tokens || is_cmd_finished(*tokens))
			return (NULL);
		cmd_node = parse_command_with_redirects(tokens);
		if (!cmd_node)
			return (NULL);
		add_command_to_pipeline(pipeline_node, cmd_node);
	}
	return (pipeline_node);
}

t_ast	*parse_logical_expr(t_token **tokens)
{
	t_logical_op	op;
	t_ast			*result;
	t_ast			*right;
	t_ast			*left;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		op = (*tokens)->type - 6;
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		if (!right)
			return (NULL);
		result = create_logical_node(op, left, right);
		if (!result)
			return (NULL);
		left = result;
	}
	return (left);
}
