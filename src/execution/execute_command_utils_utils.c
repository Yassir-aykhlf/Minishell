/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:33:23 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 10:33:48 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_not_found_error(char *path)
{
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	return (ft_strdup("NO_FILE_ERROR"));
}

char	*handle_directory_error(char *path)
{
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	return (ft_strdup("DIR_ERROR"));
}

char	*handle_permission_error(char *path)
{
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	return (ft_strdup("PERM_ERROR"));
}

char	*check_path_validity(char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) == -1)
		return (handle_not_found_error(path));
	if (S_ISDIR(file_stat.st_mode))
		return (handle_directory_error(path));
	else if (access(path, X_OK) != 0)
		return (handle_permission_error(path));
	return (path);
}
