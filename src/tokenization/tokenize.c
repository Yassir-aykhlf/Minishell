/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:01:14 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/12 23:39:59 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token));
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
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
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

void	add_operator_token(t_token **tokens, t_token_type type
						, const char *value, int *pos)
{
	add_token(tokens, create_token(type, ft_strdup(value)));
	if (ft_strlen(value) == 1)
		*pos += 1;
	else
		*pos += 2;
}

void	handle_operator(const char *input, int *pos, t_token **tokens)
{
	if (strncmp(input + *pos, "<<", 2) == 0)
		add_operator_token(tokens, TOKEN_HEREDOC, "<<", pos);
	else if (strncmp(input + *pos, ">>", 2) == 0)
		add_operator_token(tokens, TOKEN_APPEND, ">>", pos);
	else if (strncmp(input + *pos, "&&", 2) == 0)
		add_operator_token(tokens, TOKEN_AND, "&&", pos);
	else if (strncmp(input + *pos, "||", 2) == 0)
		add_operator_token(tokens, TOKEN_OR, "||", pos);
	else if (input[*pos] == '<')
		add_operator_token(tokens, TOKEN_REDIRECT_IN, "<", pos);
	else if (input[*pos] == '>')
		add_operator_token(tokens, TOKEN_REDIRECT_OUT, ">", pos);
	else if (input[*pos] == '|')
		add_operator_token(tokens, TOKEN_PIPE, "|", pos);
	else if (input[*pos] == '(')
		add_operator_token(tokens, TOKEN_PAREN_OPEN, "(", pos);
	else if (input[*pos] == ')')
		add_operator_token(tokens, TOKEN_PAREN_CLOSE, ")", pos);
}

char	*process_word_segment(const char *input, int *pos, int len)
{
	int		start;
	char	*segment;
	char	quote;
	char	*quoted_part;

	if (input[*pos] == '\'' || input[*pos] == '\"')
	{
		quote = input[*pos];
		return (extract_quoted_string(input, pos, quote));
	}
	else
	{
		start = *pos;
		while (*pos < len && !is_whitespace(input[*pos])
			&& !ft_strchr("<>|&()'\"", input[*pos]))
			(*pos)++;
		return (ft_strndup(input + start, *pos - start));
	}
}

t_token	*ft_tokenize(const char *input)
{
	int		pos;
	int		len;
	char	*word_buffer;
	char	*segment;
	t_token	*tokens;

	pos = 0;
	len = strlen(input);
	segment = NULL;
	tokens = NULL;
	while (pos < len)
	{
		while (pos < len && is_whitespace(input[pos]))
			pos++;
		if (pos >= len)
			break ;
		if (ft_strchr("<>|&()", input[pos]))
		{
			handle_operator(input, &pos, &tokens);
			continue;
		}
		word_buffer = NULL;
		while (pos < len && !is_whitespace(input[pos])
			&& !ft_strchr("<>|&()", input[pos]))
		{
			segment = process_word_segment(input, &pos, len);
			word_buffer = ft_strjoin(word_buffer, segment);
		}
		if (word_buffer)
			add_token(&tokens, create_token(TOKEN_WORD, word_buffer));
	}
	return (tokens);
}
