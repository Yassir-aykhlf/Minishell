/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:54:25 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 10:36:37 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_pure_redirections(t_ast *cmd_node)
{
	int		redir_count;
	int		redir_status;
	pid_t	pid;

	redir_count = get_redir_count(cmd_node->u_data.s_cmd.redirects);
	if (redir_count > 0)
	{
		pid = ft_fork();
		if (pid < 0)
			return (spit_error(EXIT_FAILURE, "fork", true));
		else if (pid == 0)
		{
			redir_status = redirect(cmd_node->u_data.s_cmd.redirects);
			if (redir_status != 0)
				exit(redir_status);
			exit(0);
		}
		return (wait_for_child(pid));
	}
	return (0);
}

int	exec_builtin_with_redirect(char *command_name, t_ast *cmd_node, char **argv)
{
	pid_t	pid;
	int		redir_status;
	int		status;

	pid = ft_fork();
	if (pid < 0)
		return (spit_error(EXIT_FAILURE, "fork", true));
	else if (pid == 0)
	{
		redir_status = redirect(cmd_node->u_data.s_cmd.redirects);
		if (redir_status != 0)
			exit(redir_status);
		status = execute_builtin(command_name, argv);
		exit(status);
	}
	return (wait_for_child(pid));
}

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

int	execute_command(t_ast *cmd_node)
{
	char	*full_path;
	char	*command_name;
	int		path_status;

	expand_command(cmd_node);
	if (cmd_node->u_data.s_cmd.argv == NULL)
		return (handle_pure_redirections(cmd_node));
	handle_empty_arg(cmd_node);
	command_name = get_command_name(cmd_node);
	if (!command_name || !command_name[0])
		return (0);
	if (is_builtin(command_name))
		return (handle_builtin_command(command_name, cmd_node));
	path_status = check_command_path(command_name, &full_path);
	if (path_status != 0)
		return (path_status);
	return (launch_external_command(full_path, cmd_node));
}
