/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:34:33 by arajma            #+#    #+#             */
/*   Updated: 2025/03/08 16:20:41 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Helper function to add a redirect to a command node */
void	add_redirect(t_ast *cmd_node, t_token_type type, char *file)
{
	size_t	new_size;
	t_redir	*new_redirects;

	new_size = cmd_node->data.cmd.redirect_count + 1;
	new_redirects = ft_malloc(sizeof(t_redir) * new_size);
	ft_memmove(new_redirects, cmd_node->data.cmd.redirects,
		sizeof(t_redir) * (new_size - 1));
	cmd_node->data.cmd.redirects = new_redirects;
	cmd_node->data.cmd.redirects[cmd_node->data.cmd.redirect_count].type = type;
	cmd_node->data.cmd.redirects[cmd_node
		->data.cmd.redirect_count].file = strdup(file);
	cmd_node->data.cmd.redirect_count++;
}

/* Helper function to add an argument to a command node */
void	add_argument(t_ast *cmd_node, char *arg)
{
	char	**new_args;
	size_t	arg_count;

	arg_count = 0;
	if (cmd_node->data.cmd.cmd)
		while (cmd_node->data.cmd.cmd[arg_count])
			arg_count++;
	new_args = ft_malloc(sizeof(char *) * (arg_count + 2));
	ft_memmove(new_args, cmd_node->data.cmd.cmd, sizeof(char *) * arg_count);
	cmd_node->data.cmd.cmd = new_args;
	cmd_node->data.cmd.cmd[arg_count] = ft_strdup(arg);
	cmd_node->data.cmd.cmd[arg_count + 1] = NULL;
}

/* Helper function to add a command to a pipeline node */
void	add_command_to_pipeline(t_ast *pipeline_node, t_ast *cmd_node)
{
	size_t	new_size;
	t_ast	**new_commands;

	new_size = pipeline_node->data.pipeline.count + 1;
	new_commands = ft_malloc(sizeof(t_ast *) * new_size);
	ft_memmove(new_commands, pipeline_node->data
		.pipeline.commands, sizeof(t_ast *) * (new_size - 1));
	pipeline_node->data.pipeline.commands = new_commands;
	pipeline_node->data.pipeline.commands[pipeline_node
		->data.pipeline.count] = cmd_node;
	pipeline_node->data.pipeline.count++;
}

/* C'mon it is just here for normes */
int	is_redirect(t_token *token)
{
	if (token->type == TOKEN_REDIRECT_IN
		|| token->type == TOKEN_REDIRECT_OUT
		|| token->type == TOKEN_APPEND
		|| token->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

/* C'mon it is just here for normes */
int	is_cmd_finished(t_token *token)
{
	if (token->type == TOKEN_PIPE
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_OR)
		return (1);
	return (0);
}
