/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:04:56 by arajma            #+#    #+#             */
/*   Updated: 2025/03/13 14:38:05 by yaykhlf          ###   ########.fr       */
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
	if (!*tokens || !(*tokens)->type == TOKEN_PAREN_CLOSE)
		return (NULL);
	*tokens = (*tokens)->next;
	subshell_node = create_subshell_node();
	subshell_node->data.subshell.command = command_node;
	return (subshell_node);
}

/* Function to parse a command */
t_ast	*parse_command(t_token **tokens)
{
	t_token_type	redirect_type;
	t_token			*current;
	t_ast			*cmd_node;

	cmd_node = create_command_node();
	while (*tokens)
	{
		current = *tokens;
		if (is_cmd_finished(current))
			break ;
		if (is_redirect(current))
		{
			redirect_type = current->type;
			*tokens = current->next;
			if (!*tokens || ((*tokens)->type != TOKEN_WORD))
				return (NULL);
			add_redirect(cmd_node, redirect_type, (*tokens)->value);
			*tokens = (*tokens)->next;
			continue ;
		}
		if (current->type == TOKEN_WORD)
		{
			add_argument(cmd_node, current->value);
			*tokens = current->next;
			continue ;
		}
		return (NULL);
	}
	if (!cmd_node->data.cmd.argv || !cmd_node->data.cmd.argv[0])
		return (NULL);
	return (cmd_node);
}

/* Function to parse a pipeline */
t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*pipeline_node;
	t_ast	*cmd_node;

	pipeline_node = create_pipeline_node();
	cmd_node = parse_factor(tokens);
	add_command_to_pipeline(pipeline_node, cmd_node);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		cmd_node = parse_factor(tokens);
		add_command_to_pipeline(pipeline_node, cmd_node);
	}
	if (pipeline_node->data.pipeline.count == 1)
	{
		cmd_node = pipeline_node->data.pipeline.commands[0];
		return (cmd_node);
	}
	return (pipeline_node);
}

/* Function to parse logical expressions */
t_ast	*parse_logical_expr(t_token **tokens)
{
	t_logical_op	op;

	t_ast *(result), (*right), (*left);
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

/* Main parsing function */
t_ast	*ft_parse(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (parse_logical_expr(tokens));
}
