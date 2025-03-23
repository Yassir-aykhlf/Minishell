/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:11:21 by arajma            #+#    #+#             */
/*   Updated: 2025/03/19 13:55:50 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// utility to swap nodes returns 0 in error cases
int	ft_swap_nodes(t_token *node1, t_token *node2)
{
	t_token_type	type;
	char			*value;

	if (!node1 || !node2)
		return (0);
	type = node1->type;
	node1->type = node2->type;
	node2->type = type;
	value = node1->value;
	node1->value = node2->value;
	node2->value = value;
	return (1);
}

// handeling heredoc utility to change to get multiline string from STDIN
char	*handle_heredoc(char *delim)
{
	int (i) = 0;
	char *(input), (*result) = NULL;
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
	return (result);
}

// handeling heredoc if it exists
t_token	*ft_heredoc(t_token *tokens)
{
	int (i) = 0;
	t_token *(prv), (*temp) = tokens;
	prv = NULL;
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
			if (temp->next->next)
				if (!prv || (prv && prv->type != TOKEN_WORD))
					(ft_swap_nodes(temp->next, temp->next->next)
						, ft_swap_nodes(temp, temp->next));
		}
		i++;
		prv = temp;
		temp = temp->next;
	}
	return (tokens);
}
