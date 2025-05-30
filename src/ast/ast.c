/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:04:56 by arajma            #+#    #+#             */
/*   Updated: 2025/05/04 10:42:49 by yaykhlf          ###   ########.fr       */
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

t_ast	*parse_simple_cmd(t_token **tokens)
{
	t_ast	*node;

	node = create_command_node();
	while (*tokens && !is_cmd_finished(*tokens))
	{
		t_token (*cur) = *tokens;
		if (is_redirect(cur))
		{
			t_token_type (type) = cur->type;
			*tokens = cur->next;
			if (!*tokens || (*tokens)->type != TOKEN_WORD)
				return (NULL);
			add_redirect(node, type, (*tokens)->value, (*tokens)->mask);
			*tokens = (*tokens)->next;
		}
		else if (cur->type == TOKEN_WORD)
			(add_argument(node, cur->value, cur->mask), *tokens = cur->next);
		else
			break ;
	}
	if (node->type == NODE_COMMAND && !node->u_data.s_cmd.argv
		&& !node->u_data.s_cmd.redirects)
		return (NULL);
	return (node);
}

t_ast	*parse_cmd_redir(t_token **tokens)
{
	t_ast	*node;

	if (*tokens && (*tokens)->type == TOKEN_PAREN_OPEN)
	{
		node = parse_subshell(tokens);
		if (!node)
			return (NULL);
		return (node);
	}
	return (parse_simple_cmd(tokens));
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*pipeline_node;
	t_ast	*cmd_node;

	pipeline_node = create_pipeline_node();
	if (!*tokens || is_cmd_finished(*tokens))
		return (NULL);
	cmd_node = parse_cmd_redir(tokens);
	if (!cmd_node)
		return (NULL);
	add_command_to_pipeline(pipeline_node, cmd_node);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		if (!*tokens || is_cmd_finished(*tokens))
			return (NULL);
		cmd_node = parse_cmd_redir(tokens);
		if (!cmd_node)
			return (NULL);
		add_command_to_pipeline(pipeline_node, cmd_node);
	}
	if (pipeline_node && pipeline_node->u_data.s_pipeline.commands[0]
		&& pipeline_node->u_data.s_pipeline.count == 1)
		return (pipeline_node->u_data.s_pipeline.commands[0]);
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
