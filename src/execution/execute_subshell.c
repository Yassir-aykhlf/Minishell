/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:53:13 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 17:53:41 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_subshell(t_ast *node)
{
	pid_t	pid;
	int		status;

	if (!node || node->type != NODE_SUBSHELL || !node->u_data.s_subshell.command)
		return (spit_error(EXIT_FAILURE, "Invalid subshell node", false));
	pid = fork();
	if (pid < 0)
		return (spit_error(EXIT_FAILURE, "fork", true));
	if (pid == 0)
	{
		status = execute_recursive(node->u_data.s_subshell.command);
		exit(status);
	}
	else
	{
		if (ft_waitpid(pid, &status, 0) == -1)
			return (spit_error(EXIT_FAILURE, "waitpid", true));
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
		return (EXIT_FAILURE);
	}
}
