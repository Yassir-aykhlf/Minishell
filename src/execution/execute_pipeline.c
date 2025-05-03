/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:42:53 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 19:13:29 by yaykhlf          ###   ########.fr       */
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

int	execute_pipeline(t_ast *node)
{
	int		prev_pipe_read;
	int		pipe_fds[2];
	pid_t	pid;
	int		i;
	int		pipeline_count;
	pid_t	last_pid;

	last_pid = -1;
	if (!node || node->type != NODE_PIPELINE
		|| !node->u_data.s_pipeline.commands)
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
			execute_pipeline_child(node, i, prev_pipe_read, pipe_fds);
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
