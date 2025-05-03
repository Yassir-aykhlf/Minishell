/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:54:25 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 19:14:57 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	wait_for_child(pid_t pid)
{
	int	status;

	if (ft_waitpid(pid, &status, 0) == -1)
		return (spit_error(EXIT_FAILURE, "waitpid", true));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE);
}

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

int	handle_builtin_command(char *command_name, t_ast *cmd_node)
{
	int		status;
	char	**argv;
	int		redir_count;
	int		redir_status;
	pid_t	pid;

	argv = get_argv(cmd_node->u_data.s_cmd.argv);
	if (!argv)
		return (spit_error(EXIT_FAILURE, "get_argv", true));
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
			status = execute_builtin(command_name, argv);
			exit(status);
		}
		return (wait_for_child(pid));
	}
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
	pid_t	pid;
	char	**envp;
	char	*command_name;

	expand_command(cmd_node);
	if (cmd_node->u_data.s_cmd.argv == NULL)
		return (handle_pure_redirections(cmd_node));
	command_name = cmd_node->u_data.s_cmd.argv->arg;
	if (!command_name)
		return (spit_error(EXIT_FAILURE, "No command specified", false));
	if (is_builtin(command_name))
		return (handle_builtin_command(command_name, cmd_node));
	full_path = resolve_command_path(command_name);
	if (!full_path)
		return (spit_error(127, command_name, true));
	envp = env_to_array();
	if (!envp)
		return (spit_error(EXIT_FAILURE, "env_to_array", true));
	pid = ft_fork();
	if (pid < 0)
		return (spit_error(EXIT_FAILURE, "fork", true));
	else if (pid == 0)
		execute_in_child(full_path, cmd_node, envp);
	return (wait_for_child(pid));
}
