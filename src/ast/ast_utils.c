/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:34:33 by arajma            #+#    #+#             */
/*   Updated: 2025/04/17 20:05:58 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_redirect(t_ast *node, t_token_type type, char *file, char *mask)
{
	t_redir	*new_redir;
	t_redir	*current;

	if (node->type != NODE_COMMAND)
		return ;
	new_redir = ft_malloc(sizeof(t_redir));
	new_redir->type = type;
	new_redir->file = ft_strdup(file);
	new_redir->mask = ft_strdup(mask);
	new_redir->next = NULL;
	if (!node->u_data.s_cmd.redirects)
		node->u_data.s_cmd.redirects = new_redir;
	else
	{
		current = node->u_data.s_cmd.redirects;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}

void	add_argument(t_ast *cmd_node, char *arg, char *mask)
{
	t_args	*new_arg;
	t_args	*current;

	new_arg = ft_malloc(sizeof(t_args));
	new_arg->arg = ft_strdup(arg);
	new_arg->mask = ft_strdup(mask);
	new_arg->next = NULL;
	if (!cmd_node->u_data.s_cmd.argv)
		cmd_node->u_data.s_cmd.argv = new_arg;
	else
	{
		current = cmd_node->u_data.s_cmd.argv;
		while (current->next)
			current = current->next;
		current->next = new_arg;
	}
}

void	add_command_to_pipeline(t_ast *pipeline_node, t_ast *cmd_node)
{
	size_t	new_size;
	t_ast	**new_commands;

	new_size = pipeline_node->u_data.s_pipeline.count + 1;
	new_commands = ft_malloc(sizeof(t_ast *) * new_size);
	ft_memmove(new_commands, pipeline_node->u_data
		.s_pipeline.commands, sizeof(t_ast *) * (new_size - 1));
	pipeline_node->u_data.s_pipeline.commands = new_commands;
	pipeline_node->u_data.s_pipeline.commands[pipeline_node
		->u_data.s_pipeline.count] = cmd_node;
	pipeline_node->u_data.s_pipeline.count++;
}
