/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:29:58 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 10:34:54 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_empty_arg(t_ast *cmd_node)
{
	t_args	*empty_arg;

	if (cmd_node->u_data.s_cmd.argv->arg
		&& cmd_node->u_data.s_cmd.argv->arg[0] == '\0'
		&& cmd_node->u_data.s_cmd.argv->next)
	{
		empty_arg = cmd_node->u_data.s_cmd.argv;
		cmd_node->u_data.s_cmd.argv = cmd_node->u_data.s_cmd.argv->next;
		ft_free(empty_arg->arg);
		ft_free(empty_arg->mask);
		ft_free(empty_arg);
	}
}

char	*get_command_name(t_ast *cmd_node)
{
	if (cmd_node->u_data.s_cmd.argv)
		return (cmd_node->u_data.s_cmd.argv->arg);
	return (NULL);
}

int	check_command_path(char *command_name, char **full_path)
{
	*full_path = resolve_command_path(command_name);
	if (!*full_path)
		return (spit_error(127, cat(command_name, ": command not found\n"), 0));
	if (ft_strcmp(*full_path, "DIR_ERROR") == 0)
		return (126);
	else if (ft_strcmp(*full_path, "PERM_ERROR") == 0)
		return (126);
	else if (ft_strcmp(*full_path, "NO_FILE_ERROR") == 0)
		return (127);
	return (0);
}

int	launch_external_command(char *full_path, t_ast *cmd_node)
{
	pid_t	pid;
	char	**envp;
	int		status;

	envp = env_to_array();
	if (!envp)
		return (spit_error(EXIT_FAILURE, "env_to_array", true));
	pid = ft_fork();
	if (pid < 0)
		return (spit_error(EXIT_FAILURE, "fork", true));
	else if (pid == 0)
		execute_in_child(full_path, cmd_node, envp);
	status = wait_for_child(pid);
	return (status);
}
