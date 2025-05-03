/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:50:12 by arajma            #+#    #+#             */
/*   Updated: 2025/05/03 16:58:46 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*generate_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;
	char		*counter_str;

	counter_str = ft_itoa(counter++);
	filename = ft_strjoin("/tmp/minishell_heredoc_", counter_str);
	return (filename);
}

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(SIGINT_EXIT);
}

static int	child_handle_heredoc(t_token *tokens, int fd)
{
	char *(input), *(delim) = tokens->value;
	signal(SIGINT, heredoc_sigint_handler);
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, delim) == 0)
		{
			if (!input)
				printf("warning: here-document delimited by end-of-file\
 (wanted `%s')\n", delim);
			break ;
		}
		if (to_expand(tokens->mask->mask))
			input = expand_heredoc(input);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
	}
	close(fd);
	exit(SUCCESS);
}

char	*handle_heredoc(t_token *tokens)
{
	pid_t	pid;
	char	*filename;
	int		fd;
	int		status;

	filename = generate_temp_filename();
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	pid = fork();
	if (pid < 0)
	{
		close(fd);
		unlink(filename);
		return (NULL);
	}
	if (pid == 0)
		child_handle_heredoc(tokens, fd);
	close(fd);
	ft_waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == SIGINT_EXIT)
	{
		unlink(filename);
		return (NULL);
	}
	return (filename);
}

t_token	*ft_heredoc(t_token *tokens)
{
	t_token	*temp;
	char	*heredoc_file;

	temp = tokens;
	while (temp)
	{
		if (temp->type == TOKEN_HEREDOC)
		{
			if (!temp->next || temp->next->type != TOKEN_WORD)
				return (NULL);
			heredoc_file = handle_heredoc(temp->next);
			if (!heredoc_file)
				return (NULL);
			temp->next->value = heredoc_file;
		}
		temp = temp->next;
	}
	return (tokens);
}
