/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:20:02 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 17:23:28 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	change_directory_and_update_env(char *target_dir, char *current_dir)
{
	char	*new_pwd;

	if (chdir(target_dir) == -1)
	{
		spit_error(1, "cd", true);
		return (1);
	}
	set_env_var("OLDPWD", current_dir);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		spit_error(1, "getcwd after cd", true);
		return (0);
	}
	set_env_var("PWD", new_pwd);
	free(new_pwd); 
	return (0);
}

char	*determine_cd_target_dir(char *arg)
{
	char *target_dir;

	if (!arg || ft_strcmp(arg, "~") == 0)
	{
		target_dir = get_env_value("HOME");
		if (!target_dir)
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return target_dir;
	}
	else if (ft_strcmp(arg, "-") == 0)
	{
		target_dir = get_env_value("OLDPWD");
		if (!target_dir)
			ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
		else
		{
			ft_putstr_fd(target_dir, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		return target_dir;
	}
	else
		return (arg);
}

int	builtin_cd(char **args, char **env)
{
	char	*target_dir;
	char	*current_dir;
	char	*target_arg;
	int		status;

	(void)env;
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (spit_error(1, "getcwd before cd", true));
	target_arg = args[1];
	target_dir = determine_cd_target_dir(target_arg);
	if (!target_dir)
	{
		free(current_dir);
		return (1);
	}
	status = change_directory_and_update_env(target_dir, current_dir);
	return (status);
}
