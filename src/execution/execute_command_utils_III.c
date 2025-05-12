/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils_III.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:19:41 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/12 20:19:42 by yaykhlf          ###   ########.fr       */
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
