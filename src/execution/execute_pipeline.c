/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:42:53 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 21:14:28 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_parent_pipes(int cmd_index, int *prev_pipe_read,
		int pipe_fds[2], int pipeline_count)
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

void	execute_pipeline_child(t_ast *node, int cmd_index,
		int prev_pipe_read, int pipe_fds[2])
{
	int	status;

	status = 0;
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
	status = execute_recursive(node->u_data.s_pipeline.commands[cmd_index]);
	exit(status);
}

int	wait_for_pipeline_children(int count, pid_t last_pid)
{
	int		i;
	int		status;
	int		last_status;
	pid_t	waited_pid;

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

pid_t	execute_pipeline_step(t_ast *node, t_pipeline_state *state)
{
	int		pipe_fds[2];
	pid_t	pid;

	pipe_fds[0] = -1;
	pipe_fds[1] = -1;
	if (state->index < state->count - 1)
	{
		if (pipe(pipe_fds) == -1)
			return (spit_error(EXIT_FAILURE, "pipe", true));
	}
	pid = fork();
	if (pid == -1)
		return (spit_error(EXIT_FAILURE, "fork", true));
	if (pid == 0)
		execute_pipeline_child(node, state->index,
			*state->prev_pipe_read_ptr, pipe_fds);
	else
	{
		handle_parent_pipes(state->index, state->prev_pipe_read_ptr,
			pipe_fds, state->count);
		if (state->index == state->count - 1)
			*(state->last_pid_ptr) = pid;
		return (pid);
	}
	return (-1);
}

int	execute_pipeline(t_ast *node)
{
	int					prev_pipe_read;
	pid_t				last_pid;
	pid_t				current_pid;
	t_pipeline_state	state;

	if (!node || node->type != NODE_PIPELINE
		|| !node->u_data.s_pipeline.commands)
		return (spit_error(EXIT_FAILURE, "Invalid pipeline node", false));
	state.count = node->u_data.s_pipeline.count;
	if (state.count <= 0)
		return (0);
	prev_pipe_read = -1;
	last_pid = -1;
	state.prev_pipe_read_ptr = &prev_pipe_read;
	state.last_pid_ptr = &last_pid;
	state.index = 0;
	while (state.index < state.count)
	{
		current_pid = execute_pipeline_step(node, &state);
		if (current_pid == -1)
			return (wait_for_pipeline_children(state.index, -1));
		state.index++;
	}
	return (wait_for_pipeline_children(state.count, last_pid));
}
