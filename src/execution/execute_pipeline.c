/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:42:53 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 18:33:45 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static t_pipeline_state	init_pipeline_state(t_ast *node)
{
	t_pipeline_state	state;

	state.count = node->u_data.s_pipeline.count;
	state.index = 0;
	return (state);
}

static int	check_pipeline_validity(t_ast *node)
{
	if (!node || node->type != NODE_PIPELINE
		|| !node->u_data.s_pipeline.commands)
		return (1);
	if (node->u_data.s_pipeline.count > 0
		&& node->u_data.s_pipeline.commands[0]->type == NODE_EMPTY_COMMAND)
		return (2);
	if (node->u_data.s_pipeline.count <= 0)
		return (3);
	return (0);
}

static void	init_to_minus_one(int *prev_pipe_read, pid_t *last_pid)
{
	*prev_pipe_read = -1;
	*last_pid = -1;
}

int	execute_pipeline(t_ast *node)
{
	int					prev_pipe_read;
	pid_t				last_pid;
	pid_t				current_pid;
	t_pipeline_state	state;
	int					validation;

	validation = check_pipeline_validity(node);
	if (validation == 1)
		return (spit_error(EXIT_FAILURE, "Invalid pipeline node", false));
	if (validation == 2)
		return (127);
	if (validation == 3)
		return (0);
	state = init_pipeline_state(node);
	init_to_minus_one(&prev_pipe_read, &last_pid);
	state.prev_pipe_read_ptr = &prev_pipe_read;
	state.last_pid_ptr = &last_pid;
	while (state.index < state.count)
	{
		current_pid = execute_pipeline_step(node, &state);
		if (current_pid == -1)
			return (wait_for_pipeline_children(state.index, -1));
		state.index++;
	}
	return (wait_for_pipeline_children(state.count, last_pid));
}
