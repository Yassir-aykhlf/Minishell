/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:06:49 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 10:38:00 by yaykhlf          ###   ########.fr       */
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

char	*get_path_env_value(void)
{
	char	**env;
	int		i;

	env = env_to_array();
	if (!env)
		return (NULL);
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	return (env[i] + 5);
}

char	*check_cmd_in_dir(char *dir_path, char *cmd)
{
	char	*tmp_slash;
	char	*full_path;

	tmp_slash = ft_strjoin(dir_path, "/");
	if (!tmp_slash)
	{
		spit_error(EXIT_FAILURE, "ft_strjoin", true);
		return (NULL);
	}
	full_path = ft_strjoin(tmp_slash, cmd);
	if (!full_path)
		spit_error(EXIT_FAILURE, "ft_strjoin", true);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	return (NULL);
}

char	*search_path(char *cmd)
{
	char	*path_env_value;
	char	**paths;
	char	*executable_path;
	int		i;

	path_env_value = get_path_env_value();
	if (!path_env_value)
		return (NULL);
	paths = ft_split(path_env_value, ':');
	if (!paths)
	{
		spit_error(EXIT_FAILURE, "ft_split", true);
		return (NULL);
	}
	i = 0;
	executable_path = NULL;
	while (paths[i] && !executable_path)
	{
		executable_path = check_cmd_in_dir(paths[i], cmd);
		i++;
	}
	i = 0;
	return (executable_path);
}

char	*resolve_command_path(char *command_name)
{
	char	*path;

	if (!command_name || !command_name[0])
		return (NULL);
	if (ft_strchr(command_name, '/'))
	{
		path = ft_strdup(command_name);
		if (!path)
			return (spit_error(EXIT_FAILURE, "ft_strdup", true), NULL);
		return (check_path_validity(path));
	}
	else
		path = search_path((char *)command_name);
	return (path);
}
