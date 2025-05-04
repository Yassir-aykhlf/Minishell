/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:54:25 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 18:37:07 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_builtin_command(char *command_name, t_ast *cmd_node)
{
	int		status;
	char	**argv;
	int		redir_count;

	argv = get_argv(cmd_node->u_data.s_cmd.argv);
	if (!argv)
		return (spit_error(EXIT_FAILURE, "get_argv", true));
	redir_count = get_redir_count(cmd_node->u_data.s_cmd.redirects);
	if (redir_count > 0)
		return (exec_builtin_with_redirect(command_name, cmd_node, argv));
	status = execute_builtin(command_name, argv);
	set_exit_status(status);
	return (status);
}

int	execute_in_child(char *full_path, t_ast *cmd_node, char **envp)
{
	int		redir_status;
	int		redir_count;
	char	**argv;

	redir_count = get_redir_count(cmd_node->u_data.s_cmd.redirects);
	if (redir_count > 0)
	{
		redir_status = redirect(cmd_node->u_data.s_cmd.redirects);
		if (redir_status != 0)
			exit(redir_status);
	}
	argv = get_argv(cmd_node->u_data.s_cmd.argv);
	if (!argv)
		exit(spit_error(EXIT_FAILURE, "get_argv", true));
	execve(full_path, argv, envp);
	spit_error(EXIT_FAILURE, "execve", true);
	exit(EXIT_FAILURE);
}

static int	handle_command_execution(t_ast *cmd_node, char *command_name)
{
	int		path_status;
	char	*full_path;

	if (is_builtin(command_name))
		return (handle_builtin_command(command_name, cmd_node));
	path_status = check_command_path(command_name, &full_path);
	if (path_status != 0)
		return (path_status);
	return (launch_external_command(full_path, cmd_node));
}

static int	handle_empty_command(int from_var_expansion, t_ast *cmd_node)
{
	if (from_var_expansion)
		return (0);
	cmd_node->type = NODE_EMPTY_COMMAND;
	return (spit_error(127, "command not found\n", 0));
}

int	execute_command(t_ast *cmd_node)
{
	char	*command_name;
	int		from_var_expansion;

	set_var_expansion_flag(0);
	expand_command(cmd_node);
	if (cmd_node->u_data.s_cmd.argv == NULL)
		return (handle_pure_redirections(cmd_node));
	handle_empty_arg(cmd_node);
	command_name = get_command_name(cmd_node);
	if (!command_name)
		return (0);
	from_var_expansion = *get_var_expansion_flag();
	if (!command_name[0])
		return (handle_empty_command(from_var_expansion, cmd_node));
	return (handle_command_execution(cmd_node, command_name));
}
