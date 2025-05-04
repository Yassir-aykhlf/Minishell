/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:34:02 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 15:24:20 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(void)
{
	char	*pwd;

	pwd = get_env_value("PWD");
	if (!pwd)
		return (spit_error(EXIT_FAILURE, "getcwd", true));
	ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	builtin_env(char **env)
{
	(void)env;
	print_env();
	return (0);
}

int	builtin_unset(char **args, char **env)
{
	int	i;

	(void)env;
	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		unset_env_var(args[i]);
		i++;
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(char *cmd, char **args)
{
	char	**env;

	env = env_to_array();
	if (!ft_strcmp(cmd, "echo"))
		return (builtin_echo(args));
	if (!ft_strcmp(cmd, "cd"))
		return (builtin_cd(args, env));
	if (!ft_strcmp(cmd, "pwd"))
		return (builtin_pwd());
	if (!ft_strcmp(cmd, "export"))
		return (builtin_export(args, env));
	if (!ft_strcmp(cmd, "unset"))
		return (builtin_unset(args, env));
	if (!ft_strcmp(cmd, "env"))
		return (builtin_env(env));
	if (!ft_strcmp(cmd, "exit"))
		return (builtin_exit(args));
	return (-1);
}
