/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:50:39 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/13 17:20:43 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*search_path(char *cmd, char *envp[])
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
		err_exit(EXIT_FAILURE, "ft_split");
	i = 0;
	while (path[i])
	{
		tmp_slash = ft_strjoin(path[i], "/");
		tmp = ft_strjoin(tmp_slash, cmd);
		if (!access(tmp, X_OK))
			return (tmp);
		i++;
	}
	return (NULL);
}

void	redirect(t_redir *redirects, size_t count)
{
	int	fd;
	int	flags;
	int	mode;
	int	i;

	i = 0;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	while (i < count)
	{
		if (redirects[i].type == TOKEN_REDIRECT_IN)
			flags = O_RDONLY;
		else if (redirects[i].type == TOKEN_REDIRECT_OUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else if (redirects[i].type == TOKEN_APPEND)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(redirects[i].file, flags, mode);
		if (fd == -1)
			err_exit(EXIT_FAILURE, "open");
		if (redirects[i].type == TOKEN_REDIRECT_IN)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				err_exit(EXIT_FAILURE, "dup2");
		}
		else
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				err_exit(EXIT_FAILURE, "dup2");
		}
		if (close(fd) == -1)
			err_exit(EXIT_FAILURE, "close");
		i++;
	}
}

void	redirect(t_redir *redirects, size_t count)
{
	int	fd;
	int	flags;
	int	mode;
	int	i;

	i = 0;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	while (i < count)
	{
		if (redirects[i].type == TOKEN_REDIRECT_IN)
			flags = O_RDONLY;
		else if (redirects[i].type == TOKEN_REDIRECT_OUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else if (redirects[i].type == TOKEN_APPEND)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(redirects[i].file, flags, mode);
		if (redirects[i].type == TOKEN_REDIRECT_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		i++;
	}
}

int	execute_command(t_ast *cmd, char *envp[])
{
	char	*path;
	int		status;
	pid_t	pid;
	
	if (ft_strchr(cmd->data.cmd.argv[0], '/'))
	{
		path = ft_strdup(cmd->data.cmd.argv[0]);
		if (!path)
			err_exit(EXIT_FAILURE, "ft_strdup");
	}
	else
	{
		path = search_path(cmd->data.cmd.argv[0], envp);
		if (!path)
			err_exit(EXIT_FAILURE, "search_path");
	}
	printf("path: %s\n", path);
	pid = fork();
	if (pid < 0)
		err_exit(EXIT_FAILURE, "fork");
	else if (pid == 0)
	{
		if (cmd->data.cmd.redirect_count)
			redirect(cmd->data.cmd.redirects, cmd->data.cmd.redirect_count);
		if (execve(path, cmd->data.cmd.argv, envp) == -1)
			perror("execve");
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			err_exit(EXIT_FAILURE, "waitpid");
	}
}

int	execute_recursive(t_ast *node, char *envp[])
{
	int	status;

	status = 0;
	if (node->type == NODE_COMMAND)
		status = execute_command(node, envp);
	// else if (node->type == NODE_PIPELINE)
	// 	status = execute_pipeline(node, envp);
	// else if (node->type == NODE_LOGICAL)
	// 	status = execute_logical(node, envp);
	// else if (node->type == NODE_SUBSHELL)
	// 	status = execute_subshell(node, envp);
	return (status);
}

int	ft_execute(t_ast *root, char *envp[])
{
	if (!root)
		return (EMPTY_AST);
	return (execute_recursive(root, envp));
}
