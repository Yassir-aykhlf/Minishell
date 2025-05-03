/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:56:40 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 18:09:45 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	get_target_fd(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC)
		return (STDIN_FILENO);
	if (type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND)
		return (STDOUT_FILENO);
	return (-1);
}

int	get_redirect_flags(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC)
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
	
	target_fd = get_target_fd(redir->type);
	if (target_fd == -1)
		return (spit_error(EXIT_FAILURE, "Invalid target file descriptor", false));
	
	if (dup2(fd, target_fd) == -1)
	{
		close(fd);
		return (spit_error(EXIT_FAILURE, "dup2", true));
	}
	
	if (redir->type == TOKEN_HEREDOC)
		unlink(redir->file);
	
	close(fd);
	return (0);
}

int	redirect(t_redir *redirects)
{
	int		mode;
	int		status;
	t_redir	*current;

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	status = 0;
	current = redirects;
	while (current && status == 0)
	{
		status = apply_redirection(current, mode);
		current = current->next;
	}
	return (status);
}
