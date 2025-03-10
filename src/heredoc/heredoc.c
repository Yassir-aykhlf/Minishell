/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:11:21 by arajma            #+#    #+#             */
/*   Updated: 2025/03/10 03:34:01 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// handeling heredoc utility to change to get multiline string from STDIN
char	*handle_heredoc(char *delim)
{
	int (i) = 0;
	char *(input), (*result), (*hist) = NULL;
	result = NULL;
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, delim) == 0)
			break ;
		if (i)
			result = ft_strjoin(result, "\n");
		result = ft_strjoin(result, input);
		free(input);
		i = 1;
	}
	hist = ft_strjoin(hist, "<<");
	hist = ft_strjoin(hist, delim);
	hist = ft_strjoin(hist, "\n");
	if (result)
		hist = ft_strjoin(hist, result);
	hist = ft_strjoin(hist, "\n");
	add_history(hist);
	return (result);
}

// handeling heredoc if it exists
t_token	*here_doc(t_token *tokens)
{
	t_token *(temp) = tokens;
	while (temp)
	{
		if (temp->type == TOKEN_HEREDOC)
		{
			temp->type = TOKEN_REDIRECT_IN;
			temp->value = ft_strdup("<");
			if (!temp->next || temp->next->type != TOKEN_WORD)
				return (NULL);
			temp->next->value = handle_heredoc(temp->next->value);
			if (temp->next->value == NULL)
				temp->next->value = ft_strdup("");
		}
		temp = temp->next;
	}
	return (tokens);
}
