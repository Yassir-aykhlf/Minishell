/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:04:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/02/25 08:11:14 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*take_command(void)
{
	char	*cmnd;

	cmnd = readline("Enigma$ ");
	return (cmnd);
}

char	*search_path(char *cmnd, char *envp[])
{
	char	*tmp_slash;
	char	*path_env;
	char	**path;
	char	*tmp;
	int		i;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path_env = envp[i] + 5;
	path = ft_split(path_env, ':');
	if (!path)
	{
		perror("ft_split");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (path[i])
	{
		tmp_slash = ft_strjoin(path[i], "/");
		tmp = ft_strjoin(tmp_slash, cmnd);
		free(tmp_slash);
		if (!access(tmp, X_OK))
		{
			free_split(path);
			printf("patth: %s\n", tmp);
			return (tmp);
		}
		free(tmp);
		i++;
	}
	free_split(path);
	return (NULL);
}

void	parse_command(char *cmnd, char *envp[])
{
	int		status;
	char	**args;
	char	*path;
	pid_t	pid;

	args = ft_split(cmnd, ' ');
	if (!args || !args[0])
	{
		free_split(args);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free_split(args);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		path = search_path(args[0], envp);
		if (!path)
		{
			ft_putstr_fd("command not found: ", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd("\n", 2);
			free_split(args);
			exit(EXIT_FAILURE);
		}
		if (execve(path, args, envp) == -1)
		{
			perror("execve");
			free_split(args);
			free(path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		free_split(args);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*cmnd;

	(void)argc;
	(void)argv;
	while (1)
	{
		cmnd = take_command();
		if (!cmnd)
			break ;
		parse_command(cmnd, envp);
		free(cmnd);
	}
	return (0);
}
