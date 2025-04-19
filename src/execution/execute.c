/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:50:39 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/19 17:26:13 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*search_path(char *cmd)
{
	char	*tmp_slash;
	char	*path_env;
	char	**path;
	char	*tmp;
	int		i;
	char	**env;

	env = env_to_array();
	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	path_env = env[i] + 5;
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

char	*get_arg(t_args *args, int index)
{
	int	i;

	i = 0;
	if (!args || index < 0)
		return (NULL);
	while (args && i < index)
	{
		args = args->next;
		i++;
	}
	if (args)
		return (args->arg);
	return (NULL);
}

int	get_redir_count(t_redir *redirects)
{
	int	count;

	count = 0;
	while (redirects)
	{
		count++;
		redirects = redirects->next;
	}
	return (count);
}

char	**get_argv(t_args *args)
{
	int		count;
	char	**argv;
	int		i;
	t_args	*temp;

	count = 0;
	temp = args;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	argv = ft_malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	temp = args;
	while (temp)
	{
		argv[i++] = temp->arg;
		temp = temp->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
		!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	builtin_echo(char **args)
{
	int	newline;
	int	i;
	
	newline = 1;
	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		newline = 0;
		args++;
	}
	i = 1;
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int builtin_cd(char **args, char **env)
{
	char	*target_dir;
	char	*current_dir;
	
	target_dir = NULL;
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		perror("getcwd");
		return 1;
	}
	if (!args[1] || strcmp(args[1], "~") == 0)
	{
		target_dir = get_env_value("HOME");
		if (!target_dir)
		{
			write(2, "cd: HOME not set\n", 17);
			return 1;
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		target_dir = get_env_value("OLDPWD");
		if (!target_dir)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			return 1;
		}
	}
	else
		target_dir = args[1];
	if (chdir(target_dir) == -1)
	{
		perror("cd");
		return 1;
	}
	set_env_var("OLDPWD", current_dir);
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		perror("getcwd");
		return 1;
	}
	set_env_var("PWD", current_dir);
	system("pwd");
	print_env();
	return 0;
}

int builtin_env(char **env)
{
	print_env();
	return (0);
}

int	execute_builtin(char *cmd, char **args)
{
	char	**env;

	env = env_to_array();
	if (!ft_strcmp(cmd, "echo"))
		return (builtin_echo(args));
	else if (!ft_strcmp(cmd, "cd"))
		return (builtin_cd(args, env));
	// else if (!ft_strcmp(cmd, "pwd"))
	// 	return (builtin_pwd());
	// else if (!ft_strcmp(cmd, "export"))
	// 	return (builtin_export(args, env));
	// else if (!ft_strcmp(cmd, "unset"))
	// 	return (builtin_unset(args, env));
	else if (!ft_strcmp(cmd, "env"))
		return (builtin_env(env));
	// else if (!ft_strcmp(cmd, "exit"))
	// 	return (builtin_exit(args));
	system("pwd");
	print_env();
	return (-1);
}

int	execute_command(t_ast *cmd)
{
	char	*path;
	int		status;
	char	*command;
	pid_t	pid;
	char	**env;

	env = env_to_array();
	command = get_arg(cmd->u_data.s_cmd.argv, 0);
	if (is_builtin(command))
	{
		execute_builtin(command, get_argv(cmd->u_data.s_cmd.argv));
		system("pwd");
		print_env();
		return (0);
	}
	if (ft_strchr(get_arg(cmd->u_data.s_cmd.argv, 0), '/'))
	{
		path = ft_strdup(get_arg(cmd->u_data.s_cmd.argv, 0));
		if (!path)
			return (spit_error(EXIT_FAILURE, "ft_strdup", true));
	}
	else
	{
		path = search_path(get_arg(cmd->u_data.s_cmd.argv, 0));
		if (!path)
			return (spit_error(EXIT_FAILURE, "search_path", true));
	}
	pid = fork();
	if (pid < 0)
		return (spit_error(EXIT_FAILURE, "fork", true));
	else if (pid == 0)
	{
		if (get_redir_count(cmd->u_data.s_cmd.redirects) > 0)
			status = redirect(cmd->u_data.s_cmd.redirects, 
				get_redir_count(cmd->u_data.s_cmd.redirects));
		if (execve(path, get_argv(cmd->u_data.s_cmd.argv), env) == -1)
			return (spit_error(EXIT_FAILURE, "execve", true));
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			return (spit_error(EXIT_FAILURE, "waitpid", true));
	}
	return (status);
}

int	execute_child_process(t_ast *node, int i, int prev_pipe_read, int pipe_fds[2])
{
	char	**env;

	env = env_to_array();
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
	exit(execute_recursive(node->u_data.s_pipeline.commands[i]));
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

int	execute_pipeline(t_ast *node)
{
	int		prev_pipe_read;
	int		pipe_fds[2];
	int		status;
	pid_t	pid;
	int		i;
	char	**env;

	env = env_to_array();
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
			execute_child_process(node, i, prev_pipe_read, pipe_fds);
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

int	execute_recursive(t_ast *node)
{
	int	status;
	char	**env;

	env = env_to_array();
	status = 0;
	if (node->type == NODE_COMMAND){
		status = execute_command(node);
		system("pwd");
		print_env();
	}
	else if (node->type == NODE_PIPELINE)
		status = execute_pipeline(node);
	// else if (node->type == NODE_LOGICAL)
	// 	status = execute_logical(node, env);
	// else if (node->type == NODE_SUBSHELL)
	// 	status = execute_subshell(node, env);
	return (status);
}

int	ft_execute(t_ast *root)
{
	char	**env;

	env = env_to_array();
	print_env();
	if (!root)
		return (EMPTY_AST);
	return (execute_recursive(root));
}
