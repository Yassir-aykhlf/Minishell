/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:34:33 by arajma            #+#    #+#             */
/*   Updated: 2025/03/14 21:21:13 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Helper function to get redirects info based on node type */
static void	get_redirect_info(t_ast *node, t_redir **redirects, size_t *count)
{
	if (node->type == NODE_COMMAND)
	{
		*redirects = node->u_data.s_cmd.redirects;
		*count = node->u_data.s_cmd.redirect_count;
	}
	else if (node->type == NODE_SUBSHELL)
	{
		*redirects = node->u_data.s_subshell.redirects;
		*count = node->u_data.s_subshell.redirect_count;
	}
	else
	{
		*redirects = NULL;
		*count = 0;
	}
}

/* Function to add a redirect to a node */
void	add_redirect(t_ast *node, t_token_type type, char *file)
{
	t_redir	*old_redirects;
	t_redir	*new_redirects;
	size_t	old_count;

	get_redirect_info(node, &old_redirects, &old_count);
	new_redirects = ft_malloc(sizeof(t_redir) * (old_count + 1));
	ft_memmove(new_redirects, old_redirects, sizeof(t_redir) * old_count);
	new_redirects[old_count].type = type;
	new_redirects[old_count].file = ft_strdup(file);
	if (node->type == NODE_COMMAND)
	{
		node->u_data.s_cmd.redirects = new_redirects;
		node->u_data.s_cmd.redirect_count = old_count + 1;
	}
	else if (node->type == NODE_SUBSHELL)
	{
		node->u_data.s_subshell.redirects = new_redirects;
		node->u_data.s_subshell.redirect_count = old_count + 1;
	}
}

/* Helper function to add an argument to a command node */
void	add_argument(t_ast *cmd_node, char *arg)
{
	char	**new_args;
	size_t	arg_count;

	arg_count = 0;
	if (cmd_node->u_data.s_cmd.argv)
		while (cmd_node->u_data.s_cmd.argv[arg_count])
			arg_count++;
	new_args = ft_malloc(8 * (arg_count + 2));
	ft_memmove(new_args, cmd_node->u_data.s_cmd.argv, 8 * arg_count);
	cmd_node->u_data.s_cmd.argv = new_args;
	cmd_node->u_data.s_cmd.argv[arg_count] = ft_strdup(arg);
	cmd_node->u_data.s_cmd.argv[arg_count + 1] = NULL;
}

/* Helper function to add a command to a pipeline node */
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
