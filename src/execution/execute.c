/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:50:39 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/27 13:01:56 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*get_exit_status(void)
{
	static int exit_status = 0;

	return (&exit_status);
}

void	set_exit_status(int status)
{
	int	*exit_status_ptr;

	exit_status_ptr = get_exit_status();
	*exit_status_ptr = status;
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
	int	i;

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

int	get_redirect_flags(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN)
		return (O_RDONLY);
	if (type == TOKEN_REDIRECT_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	if (type == TOKEN_APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND);
	return (-1);
}

int	apply_redirection(t_redir *redir, int mode)
{
	int	fd;
	int	flags;
	int	target_fd;

	flags = get_redirect_flags(redir->type);
	if (flags == -1)
		return (spit_error(EXIT_FAILURE, "Unknown redirection type", false));
	fd = open(redir->file, flags, mode);
	if (fd == -1)
		return (spit_error(EXIT_FAILURE, "open", true));
	if (redir->type == TOKEN_REDIRECT_IN)
		target_fd = STDIN_FILENO;
	else
		target_fd = STDOUT_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		close(fd);
		return (spit_error(EXIT_FAILURE, "dup2", true));
	}
	if (close(fd) == -1)
		return (spit_error(EXIT_FAILURE, "close", true));
	return (0);
}

int	redirect(t_redir *redirects, size_t count)
{
	size_t	i;
	int		mode;
	int		status;

	i = 0;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	status = 0;
	while (i < count && status == 0)
	{
		status = apply_redirection(&redirects[i], mode);
		i++;
	}
	return (status);
}

char	*get_arg(t_args *args, int index)
{
	int i;

	i = 0;
	if (index < 0)
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
	int count;

	count = 0;
	while (redirects)
	{
		count++;
		redirects = redirects->next;
	}
	return (count);
}

int	count_args(t_args *args)
{
	int	count;

	count = 0;
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}

void	fill_argv_from_list(char **argv, t_args *args)
{
	int i;

	i = 0;
	while (args)
	{
		argv[i++] = args->arg;
		args = args->next;
	}
	argv[i] = NULL;
}

char	**get_argv(t_args *args)
{
	int	arg_count;
	char	**argv;

	arg_count = count_args(args);
	argv = ft_malloc(sizeof(char *) * (arg_count + 1));
	if (!argv)
	{
		spit_error(EXIT_FAILURE, "ft_malloc", true);
		return (NULL);
	}
	fill_argv_from_list(argv, args);
	return (argv);
}

int is_builtin(char *cmd)
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

char **handle_echo_newline_flag(char **args, int *newline)
{
	*newline = 1;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		*newline = 0;
		return (args + 1);
	}
	return (args);
}

void	print_echo_args(char **args)
{
	int i;

	i = 1;
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

char	*expand_variable(char *arg)
{
	char	*expanded_arg;
	char	*var_name;
	char	*var_value;

	if (arg[0] == '$')
	{
		var_name = arg + 1;
		var_value = get_env_value(var_name);
		if (var_value)
			return (ft_strdup(var_value));
	}
	return (NULL);
}

int	builtin_echo(char **args)
{
	int		i;
	int		newline;
	char	**args_to_print;
	char	*expanded_arg;

	args_to_print = handle_echo_newline_flag(args, &newline);
	i = 1;
	while (args_to_print[i])
	{
		expanded_arg = expand_variable(args_to_print[i]);
		if (expanded_arg)
			ft_putstr_fd(expanded_arg, STDOUT_FILENO);
		else
			ft_putstr_fd(args_to_print[i], STDOUT_FILENO);
		if (args_to_print[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
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
			ft_putstr_fd(target_dir, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		return target_dir;
	}
	else
		return (arg);
}

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

int	builtin_env(char **env)
{
	(void)env;
	print_env();
	return (0);
}

int	builtin_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (spit_error(EXIT_FAILURE, "getcwd", true));
	ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(pwd);
	return (0);
}

void	builtin_export_print(char **env)
{
	t_env	**env_list_head;
	t_env	*current;

	(void)env;
	env_list_head = get_env_list();
	if (!env_list_head)
		return;
	current = *env_list_head;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

bool	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '_');
}

int	is_valid_identifier(char *key, char *full_arg)
{
	int	j;

	if (!((key[0] >= 'a' && key[0] <= 'z') ||
		(key[0] >= 'A' && key[0] <= 'Z') || key[0] == '_'))
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(full_arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	j = 1;
	while (key[j])
	{
		if (!is_valid_var_char(key[j]))
		{
			ft_putstr_fd("export: `", STDERR_FILENO);
			ft_putstr_fd(full_arg, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		j++;
	}
	return (0);
}

int	validate_export(char **args)
{
	int		i;
	char	*key;
	char	*equals_pos;
	int		status;

	if (!args)
		return (EXIT_FAILURE);
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!args[i][0])
		{
			i++;
			continue;
		}
		equals_pos = ft_strchr(args[i], '=');
		if (equals_pos)
		{
			key = ft_strndup(args[i], equals_pos - args[i]);
			if (!key) return (spit_error(EXIT_FAILURE, "ft_strndup", true));
		}
		else
			key = args[i];
		if (is_valid_identifier(key, args[i]) != 0)
			status = EXIT_FAILURE;
		i++;
	}
	return (status);
}
/////////////////////////////////////////////////////////////////////////////////////////

void	parse_and_set_export_var(char *arg)
{
	char	*key;
	char	*value;
	char	*equals_pos;

	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
	{
		key = ft_strndup(arg, equals_pos - arg);
		if (!key)
		{
			spit_error(EXIT_FAILURE, "ft_strndup", true);
			return;
		}
		value = equals_pos + 1;
		set_env_var(key, value);
	}
	else
	{
		key = arg;
		set_env_var(key, NULL);
	}
}

int	builtin_export(char **args, char **env)
{
	int i;

	(void)env;
	if (validate_export(args) != 0)
		return (EXIT_FAILURE);
	if (!args[1])
	{
		builtin_export_print(NULL);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (args[i][0])
			parse_and_set_export_var(args[i]);
		i++;
	}
	return (0);
}

int	builtin_unset(char **args, char **env)
{
	int i;

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

bool is_child_process(void)
{
	static pid_t original_pid = 0;

	if (original_pid == 0)
		original_pid = getpid();
	return getpid() != original_pid;
}

int	parse_exit_status(char *arg, int *status)
{
	if (arg)
	{
		if (ft_isdigit((unsigned char)arg[0]) ||
			(arg[0] == '-' && ft_isdigit((unsigned char)arg[1])))
		{
			*status = ft_atoi(arg);
		}
		else
		{
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			*status = 2;
			return (2);
		}
	}
	else
		*status = 0;
	return (0);
}

int	builtin_exit(char **args)
{
	int	status;
	int	parse_result;

	parse_result = parse_exit_status(args[1], &status);
	if (parse_result != 0)
		return (parse_result);
	if (!is_child_process())
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(status & 0xFF);
	return (EXIT_FAILURE);
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

char *resolve_command_path(char *command_name)
{
	char *path;

	if (!command_name || !command_name[0])
		return (NULL);
	if (ft_strchr(command_name, '/'))
	{
		path = ft_strdup(command_name);
		if (!path)
			spit_error(EXIT_FAILURE, "ft_strdup", true);
		else if (access(path, X_OK) != 0)
		{
			spit_error(127, command_name, true);
			path = NULL;
		}
	}
	else
	{
		path = search_path((char *)command_name);
	}
	return (path);
}

static int execute_in_child(char *full_path, t_ast *cmd_node, char **envp)
{
	int		redir_status;
	int		redir_count;
	char	**argv;

	redir_count = get_redir_count(cmd_node->u_data.s_cmd.redirects);
	if (redir_count > 0)
	{
		redir_status = redirect(cmd_node->u_data.s_cmd.redirects, redir_count);
		if (redir_status != 0)
			exit(redir_status);
	}
	argv = get_argv(cmd_node->u_data.s_cmd.argv);
	if (!argv)
		exit(spit_error(EXIT_FAILURE, "get_argv", true));
	execve(full_path, argv, envp);
	spit_error(EXIT_FAILURE, "execve", true);
	exit(EXIT_FAILURE);
}

int wait_for_child(pid_t pid)
{
	int status;

	if (ft_waitpid(pid, &status, 0) == -1)
		return (spit_error(EXIT_FAILURE, "waitpid", true));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE);
}

int	execute_command(t_ast *cmd_node)
{
	char	*full_path;
	pid_t	pid;
	char	**envp;
	char	*command_name;
	int		status;
	char	**argv;

	expand_command(cmd_node);
	command_name = cmd_node->u_data.s_cmd.argv->arg;
	if (!command_name)
		return (spit_error(EXIT_FAILURE, "No command specified", false));
	if (is_builtin(command_name))
	{
		argv = get_argv(cmd_node->u_data.s_cmd.argv);
		if (!argv)
			return spit_error(EXIT_FAILURE, "get_argv", true);
		status = execute_builtin(command_name, argv);
		set_exit_status(status);
		return status;
	}
	full_path = resolve_command_path(command_name);
	if (!full_path)
		return (127);
	envp = env_to_array();
	if (!envp)
		return (spit_error(EXIT_FAILURE, "env_to_array", true));
	pid = ft_fork();
	if (pid < 0)
		return (spit_error(EXIT_FAILURE, "fork", true));
	else if (pid == 0)
		execute_in_child(full_path, cmd_node, envp);
	return (wait_for_child(pid));
}
///////////////////////////////////////////////////////////////////////////////////////////

void	execute_pipeline_child(t_ast *node, int cmd_index, int prev_pipe_read, int pipe_fds[2])
{
	if (prev_pipe_read != -1)
	{
		if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
			exit(spit_error(EXIT_FAILURE, "dup2 stdin", true));
		close(prev_pipe_read);
	}
	if (cmd_index < node->u_data.s_pipeline.count - 1)
	{
		close(pipe_fds[0]);
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
			exit(spit_error(EXIT_FAILURE, "dup2 stdout", true));
		close(pipe_fds[1]);
	}
	else
	{
		if (pipe_fds[0] != -1)
			close(pipe_fds[0]);
		if (pipe_fds[1] != -1)
			close(pipe_fds[1]);
	}
	exit(execute_recursive(node->u_data.s_pipeline.commands[cmd_index]));
}

void	handle_parent_pipes(int cmd_index, int *prev_pipe_read, int pipe_fds[2], int pipeline_count)
{
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (cmd_index < pipeline_count - 1)
	{
		close(pipe_fds[1]);
		*prev_pipe_read = pipe_fds[0];
	}
	else
	{
		if (pipe_fds[0] != -1)
			close(pipe_fds[0]);
		*prev_pipe_read = -1;
	}
}

int wait_for_pipeline_children(int count, pid_t last_pid)
{
	int i;
	int status;
	int last_status;
	pid_t waited_pid;

	i = 0;
	last_status = 0;
	while (i < count)
	{
		waited_pid = wait(&status);
		if (waited_pid == -1)
			spit_error(EXIT_FAILURE, "wait", true);
		else if (waited_pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
			else
				last_status = EXIT_FAILURE;
		}
		i++;
	}
	return (last_status);
}

int	execute_pipeline(t_ast *node)
{
	int		prev_pipe_read;
	int		pipe_fds[2];
	pid_t	pid;
	int		i;
	int		pipeline_count;
	pid_t	last_pid = -1;

	if (!node || node->type != NODE_PIPELINE || !node->u_data.s_pipeline.commands)
		return (spit_error(EXIT_FAILURE, "Invalid pipeline node", false));
	pipeline_count = node->u_data.s_pipeline.count;
	if (pipeline_count <= 0)
		return (0);
	prev_pipe_read = -1;
	i = 0;
	while (i < pipeline_count)
	{
		if (i < pipeline_count - 1)
		{
			if (pipe(pipe_fds) == -1)
			{
				if (prev_pipe_read != -1)
					close(prev_pipe_read);
				return (spit_error(EXIT_FAILURE, "pipe", true));
			}
		}
		else
		{
			pipe_fds[0] = -1;
			pipe_fds[1] = -1;
		}
		pid = fork();
		if (pid == -1)
		{
			if (prev_pipe_read != -1)
				close(prev_pipe_read);
			if (pipe_fds[0] != -1)
				close(pipe_fds[0]);
			if (pipe_fds[1] != -1)
				close(pipe_fds[1]);
			return (spit_error(EXIT_FAILURE, "fork", true));
		}
		if (pid == 0)
		{
			execute_pipeline_child(node, i, prev_pipe_read, pipe_fds);
		}
		else
		{
			handle_parent_pipes(i, &prev_pipe_read, pipe_fds, pipeline_count);
			if (i == pipeline_count - 1)
				last_pid = pid;
		}
		i++;
	}
	return (wait_for_pipeline_children(pipeline_count, last_pid));
}
///////////////////////////////////////////////////////////////////////////////////////////

int execute_logical(t_ast *node)
{
	int	left_status;
	int	right_status;

	if (!node || node->type != NODE_LOGICAL || !node->u_data.s_op.left)
		return (spit_error(EXIT_FAILURE, "Invalid logical node", false));
	left_status = execute_recursive(node->u_data.s_op.left);
	if (node->u_data.s_op.operat == LOGICAL_AND)
	{
		if (left_status == 0 && node->u_data.s_op.right)
			right_status = execute_recursive(node->u_data.s_op.right);
		else
			right_status = left_status;
	}
	else if (node->u_data.s_op.operat == LOGICAL_OR)
	{
		if (left_status != 0 && node->u_data.s_op.right)
			right_status = execute_recursive(node->u_data.s_op.right);
		else
			right_status = left_status;
	}
	else
		return (spit_error(EXIT_FAILURE, "Unknown logical operator", false));
	return (right_status);
}

int execute_subshell(t_ast *node)
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

int	execute_recursive(t_ast *node)
{
	int status;

	if (!node)
		return (EMPTY_AST);
	status = 0;
	if (node->type == NODE_COMMAND)
		status = execute_command(node);
	else if (node->type == NODE_PIPELINE)
		status = execute_pipeline(node);
	else if (node->type == NODE_LOGICAL)
		status = execute_logical(node);
	else if (node->type == NODE_SUBSHELL)
		status = execute_subshell(node);
	else
		status = spit_error(EXIT_FAILURE, "Unknown AST node type", false);
	set_exit_status(status);
	return (status);
}

int ft_execute(t_ast *root)
{
	if (!root)
		return (EMPTY_AST);
	return (execute_recursive(root));
}
