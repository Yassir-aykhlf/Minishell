/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:04:56 by arajma            #+#    #+#             */
/*   Updated: 2025/03/16 12:21:40 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Function to parse a subshell */
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

/* Function to parse a simple command */
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

/* Parse a command with potential redirections */
t_ast	*parse_command_with_redirects(t_token **tokens)
{
	t_ast	*node;

	if (*tokens && (*tokens)->type == TOKEN_PAREN_OPEN)
		node = parse_subshell(tokens);
	else
		node = parse_simple_command(tokens);
	return (handle_redirections(node, tokens));
}

/* Function to parse a pipeline */
t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*pipeline_node;
	t_ast	*cmd_node;

	pipeline_node = create_pipeline_node();
	cmd_node = parse_command_with_redirects(tokens);
	add_command_to_pipeline(pipeline_node, cmd_node);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		cmd_node = parse_command_with_redirects(tokens);
		add_command_to_pipeline(pipeline_node, cmd_node);
	}
	if (pipeline_node->u_data.s_pipeline.count == 1)
	{
		cmd_node = pipeline_node->u_data.s_pipeline.commands[0];
		return (cmd_node);
	}
	return (pipeline_node);
}

/* Function to parse logical expressions */
t_ast	*parse_logical_expr(t_token **tokens)
{
	t_logical_op	op;
	t_ast			*result;
	t_ast			*right;
	t_ast			*left;

	left = parse_pipeline(tokens);
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		op = (*tokens)->type - 6;
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		result = create_logical_node(op, left, right);
		left = result;
	}
	return (left);
}
