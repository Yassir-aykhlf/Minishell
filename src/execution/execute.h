/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:50:42 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 14:39:33 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "../../src/ast/ast.h"
# include <fcntl.h>
# include <sys/stat.h>

# define EMPTY_AST 1

typedef struct s_pipeline_state
{
	int		index;
	int		count;
	int		*prev_pipe_read_ptr;
	pid_t	*last_pid_ptr;
}	t_pipeline_state;

bool	is_valid_var_char(char c);
int		ft_execute(t_ast *root);
int		execute_command(t_ast *cmd);
int		execute_pipeline(t_ast *node);
int		execute_recursive(t_ast *node);
int		redirect(t_redir *redirects);
int		change_directory_and_update_env(char *target_dir, char *current_dir);
char	*determine_cd_target_dir(char *arg);
int		builtin_cd(char **args, char **env);
char	**handle_echo_newline_flag(char **args, int *newline);
void	print_echo_args(char **args);
int		builtin_echo(char **args);
int		builtin_pwd(void);
int		builtin_env(char **env);
int		builtin_unset(char **args, char **env);
int		is_builtin(char *cmd);
int		execute_builtin(char *cmd, char **args);
bool	is_valid_var_char(char c);
int		is_valid_identifier(char *key, char *full_arg);
int		validate_export(char **args);
void	builtin_export_print(char **env);
int		builtin_export(char **args, char **env);
void	parse_and_set_export_var(char *arg);
int		parse_exit_status(char *arg, int *status);
int		builtin_exit(char **args);
void	handle_parent_pipes(int cmd_index, int *prev_pipe_read,
			int pipe_fds[2], int pipeline_count);
void	execute_pipeline_child(t_ast *node, int cmd_index,
			int prev_pipe_read, int pipe_fds[2]);
int		wait_for_pipeline_children(int count, pid_t last_pid);
int		execute_pipeline_step(t_ast *node, t_pipeline_state *state);
int		execute_pipeline(t_ast *node);
int		execute_logical(t_ast *node);
int		execute_subshell(t_ast *node);
int		get_redir_count(t_redir *redirects);
int		get_target_fd(t_token_type type);
int		get_redirect_flags(t_token_type type);
int		apply_redirection(t_redir *redir, int mode);
int		redirect(t_redir *redirects);
char	*cat(char *s1, char *s2);
int		wait_for_child(pid_t pid);
int		handle_pure_redirections(t_ast *cmd_node);
int		handle_builtin_command(char *command_name, t_ast *cmd_node);
int		execute_in_child(char *full_path, t_ast *cmd_node, char **envp);
int		execute_command(t_ast *cmd_node);
char	*get_path_env_value(void);
char	*check_cmd_in_dir(char *dir_path, char *cmd);
char	*search_path(char *cmd);
char	*resolve_command_path(char *command_name);
int		*get_exit_status(void);
void	set_exit_status(int status);
bool	is_child_process(void);
void	fill_argv_from_list(char **argv, t_args *args);
char	**get_argv(t_args *args);
int		count_args(t_args *args);
int		exec_builtin_with_redirect(char *command_name,
			t_ast *cmd_node, char **argv);
char	*handle_not_found_error(char *path);
char	*handle_directory_error(char *path);
char	*handle_permission_error(char *path);
char	*check_path_validity(char *path);
void	handle_empty_arg(t_ast *cmd_node);
char	*get_command_name(t_ast *cmd_node);
int		check_command_path(char *command_name, char **full_path);
int		launch_external_command(char *full_path, t_ast *cmd_node);
int		validate_export_append(char *arg, char *append_pos);
int		validate_export_equals(char *arg, char *equals_pos);
int		validate_single_export_arg(char *arg);

#endif
