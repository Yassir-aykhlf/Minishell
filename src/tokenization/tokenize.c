/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:55:37 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/05 14:40:20 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

char	*extract_quoted_string(const char *input, int *pos, char quote)
{
	int		start;
	int		end;
	char	*str;

	start = *pos + 1;
	end = start;
	while (input[end] && input[end] != quote)
		end++;
	str = ft_strndup(input + start, end - start);
	if (input[end] == quote)
		*pos = end + 1;
	else
		*pos = end;
	return (str);
}

void	handle_operator(const char *input, int *pos, t_token **tokens)
{
	if (strncmp(input + *pos, "<<", 2) == 0)
	{
		add_token(tokens, create_token(TOKEN_HEREDOC, strdup("<<")));
		*pos += 2;
	}
	else if (strncmp(input + *pos, ">>", 2) == 0)
	{
		add_token(tokens, create_token(TOKEN_APPEND, strdup(">>")));
		*pos += 2;
	}
	else if (strncmp(input + *pos, "&&", 2) == 0)
	{
		add_token(tokens, create_token(TOKEN_AND, strdup("&&")));
		*pos += 2;
	}
	else if (strncmp(input + *pos, "||", 2) == 0)
	{
		add_token(tokens, create_token(TOKEN_OR, strdup("||")));
		*pos += 2;
	}
	else if (input[*pos] == '<')
	{
		add_token(tokens, create_token(TOKEN_REDIRECT_IN, strdup("<")));
		(*pos)++;
	}
	else if (input[*pos] == '>')
	{
		add_token(tokens, create_token(TOKEN_REDIRECT_OUT, strdup(">")));
		(*pos)++;
	}
	else if (input[*pos] == '|')
	{
		add_token(tokens, create_token(TOKEN_PIPE, strdup("|")));
		(*pos)++;
	}
}

t_token	*ft_tokenize(const char *input)
{
	t_token	*tokens;
	int		pos;
	int		len;
	int		start;
	char	*value;

	tokens = NULL;
	pos = 0;
	len = strlen(input);
	while (pos < len)
	{
		while (pos < len && is_whitespace(input[pos]))
			pos++;
		if (pos >= len)
			break ;
		if (input[pos] == '\'')
		{
			value = extract_quoted_string(input, &pos, '\'');
			add_token(&tokens, create_token(TOKEN_SINGLE_QUOTED, value));
		}
		else if (input[pos] == '"')
		{
			value = extract_quoted_string(input, &pos, '"');
			add_token(&tokens, create_token(TOKEN_DOUBLE_QUOTED, value));
		}
		else if (ft_strchr("<>|&", input[pos]))
		{
			handle_operator(input, &pos, &tokens);
		}
		else
		{
			start = pos;
			while (pos < len && !is_whitespace(input[pos])
				&& !ft_strchr("<>|&'\"", input[pos]))
				pos++;
			value = ft_strndup(input + start, pos - start);
			add_token(&tokens, create_token(TOKEN_WORD, value));
		}
	}
	return (tokens);
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}
