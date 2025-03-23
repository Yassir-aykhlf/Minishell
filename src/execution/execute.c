/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:50:39 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/23 22:04:46 by yaykhlf          ###   ########.fr       */
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
		spit_error(EXIT_FAILURE, "ft_split", true);
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

int	redirect(t_redir *redirects, size_t count)
{
	int		fd;
	int		flags;
	int		mode;
	size_t	i;

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
			return (spit_error(EXIT_FAILURE, "open", true));
		if (redirects[i].type == TOKEN_REDIRECT_IN)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				return (spit_error(EXIT_FAILURE, "dup2", true));
		}
		else
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (spit_error(EXIT_FAILURE, "dup2", true));
		}
		if (close(fd) == -1)
			return (spit_error(EXIT_FAILURE, "close", true));
		i++;
	}
	return (0);
}

int	execute_command(t_ast *cmd, char *envp[])
{
	char	*path;
	int		status;
	pid_t	pid;
	
	if (ft_strchr(cmd->u_data.s_cmd.argv[0], '/'))
	{
		path = ft_strdup(cmd->u_data.s_cmd.argv[0]);
		if (!path)
			return (spit_error(EXIT_FAILURE, "ft_strdup", true));
	}
	else
	{
		path = search_path(cmd->u_data.s_cmd.argv[0], envp);
		if (!path)
			return (spit_error(EXIT_FAILURE, "search_path", true));
	}
	pid = fork();
	if (pid < 0)
		return (spit_error(EXIT_FAILURE, "fork", true));
	else if (pid == 0)
	{
		if (cmd->u_data.s_cmd.redirect_count)
			status = redirect(cmd->u_data.s_cmd.redirects, cmd->u_data.s_cmd.redirect_count);
		if (execve(path, cmd->u_data.s_cmd.argv, envp) == -1)
			return (spit_error(EXIT_FAILURE, "execve", true));
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			return (spit_error(EXIT_FAILURE, "waitpid", true));
	}
	return (status);
}

int	execute_child_process(t_ast *node, int i, int prev_pipe_read, int pipe_fds[2], char *envp[])
{
	if (prev_pipe_read != -1)
	{
		if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
			exit(spit_error(EXIT_FAILURE, "dup2", true));
		close(prev_pipe_read);
	}
	if (i < node->u_data.s_pipeline.count - 1)
	{
		close(pipe_fds[0]);
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
			exit(spit_error(EXIT_FAILURE, "dup2", true));
		close(pipe_fds[1]);
	}
	exit(execute_recursive(node->u_data.s_pipeline.commands[i], envp));
	return (EXIT_FAILURE);
}

int	handle_parent_process(int i, int *prev_pipe_read, int pipe_fds[2], t_ast *node)
{
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (i < node->u_data.s_pipeline.count - 1)
	{
		close(pipe_fds[1]);
		*prev_pipe_read = pipe_fds[0];
	}
	return (0);
}

int	validate_pipeline(t_ast *node)
{
	if (!node || node->type != NODE_PIPELINE || node->u_data.s_pipeline.count < 1)
		return (spit_error(EXIT_FAILURE, "Invalid pipeline", false));
	return (0);
}

int	execute_pipeline(t_ast *node, char *envp[])
{
	int		prev_pipe_read;
	int		pipe_fds[2];
	int		status;
	pid_t	pid;
	int		i;

	status = 0;
	prev_pipe_read = -1;
	if (validate_pipeline(node) != 0)
		return (EXIT_FAILURE);
	i = 0;
	while (i < node->u_data.s_pipeline.count)
	{
		if (i < node->u_data.s_pipeline.count - 1)
			if (pipe(pipe_fds) == -1)
				return (spit_error(EXIT_FAILURE, "pipe", true));
		pid = fork();
		if (pid == -1)
			return (spit_error(EXIT_FAILURE, "fork", true));
		if (pid == 0)
			execute_child_process(node, i, prev_pipe_read, pipe_fds, envp);
		else
			handle_parent_process(i, &prev_pipe_read, pipe_fds, node);
		i++;
	}
	i = 0;
	while (i < node->u_data.s_pipeline.count)
	{
		wait(&status);
		i++;
	}
	return (WEXITSTATUS(status));
}

int	execute_recursive(t_ast *node, char *envp[])
{
	int	status;

	status = 0;
	if (node->type == NODE_COMMAND)
		status = execute_command(node, envp);
	else if (node->type == NODE_PIPELINE)
		status = execute_pipeline(node, envp);
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
