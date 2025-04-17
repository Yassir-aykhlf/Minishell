/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:01:14 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/17 19:05:08 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value, char *mask)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->mask = mask;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	current = *head;
	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = new_token;
}

char	*extract_quoted_string(const char *input, int *pos, char quote, char **mask)
{
	int		start;
	int		end;
	char	*str;
	char	*local_mask;

	start = *pos + 1;
	end = start;
	while (input[end] && input[end] != quote)
		end++;
	str = ft_strndup(input + start, end - start);
	local_mask = ft_calloc(sizeof(char), end - start + 1);
	if (!str || !local_mask)
		return (NULL);
	if (quote == '\'')
		ft_memset(local_mask, 'S', end - start);
	else
		ft_memset(local_mask, 'D', end - start);
	*mask = local_mask;
	if (input[end] == quote)
		*pos = end + 1;
	else
		*pos = end;
	return (str);
}

void	add_operator_token(t_token **tokens, t_token_type type, const char *value, int *pos)
{
	char *mask;
	mask = ft_calloc(ft_strlen(value) + 1, sizeof(char));
	if (mask)
		ft_memset(mask, 'N', ft_strlen(value));
	add_token(tokens, create_token(type, ft_strdup(value), mask));
	*pos += ft_strlen(value);
}

void	process_operator(const char *input, int *pos, t_token **tokens)
{
	for (int i = 0; i < (int)(sizeof(s_operators) / sizeof(s_operators[0])); i++)
	{
		if (strncmp(input + *pos, s_operators[i].symbol, ft_strlen(s_operators[i].symbol)) == 0)
		{
			add_operator_token(tokens, s_operators[i].type, s_operators[i].symbol, pos);
			return;
		}
	}
}

char	*process_word_segment(const char *input, int *pos, int len, char **segment_mask)
{
	int		start;
	char	quote;
	char	*segment = NULL;

	if (input[*pos] == '\'' || input[*pos] == '\"')
	{
		quote = input[*pos];
		segment = extract_quoted_string(input, pos, quote, segment_mask);
	}
	else
	{
		start = *pos;
		while (*pos < len && !is_whitespace(input[*pos]) && !ft_strchr("<>|&()'\"", input[*pos]))
			(*pos)++;
		segment = ft_strndup(input + start, *pos - start);
		*segment_mask = ft_calloc(*pos - start + 1, sizeof(char));
		if (*segment_mask)
			ft_memset(*segment_mask, 'N', *pos - start);
	}
	return segment;
}

void	skip_whitespace(const char *input, int *pos)
{
	while (input[*pos] && is_whitespace(input[*pos]))
		(*pos)++;
}

void	print_tokens(t_token *tokens)
{
	t_token	*current = tokens;

	while (current)
	{
		printf("Token: %s, Mask: %s\n", current->value, current->mask);
		current = current->next;
	}
}

t_token	*ft_tokenize(const char *input)
{
	int		pos;
	int		len;
	char	*word_buff = NULL;
	char	*mask_buff = NULL;
	char	*segment;
	char	*segment_mask;
	char	*temp_word;
	char	*temp_mask;
	t_token	*tokens;

	pos = 0;
	len = ft_strlen(input);
	tokens = NULL;
	while (pos < len)
	{
		skip_whitespace(input, &pos);
		if (pos >= len)
			break;
		if (ft_strchr("<>|&()", input[pos]))
		{
			process_operator(input, &pos, &tokens);
			continue;
		}
		word_buff = NULL;
		mask_buff = NULL;
		while (pos < len && !is_whitespace(input[pos]) && !ft_strchr("<>|&()", input[pos])) {
			segment_mask = NULL;
			segment = process_word_segment(input, &pos, len, &segment_mask);
			if (segment)
			{
				if (word_buff == NULL)
				{
					word_buff = segment;
					mask_buff = segment_mask;
				}
				else
				{
					temp_word = word_buff;
					temp_mask = mask_buff;
					word_buff = ft_strjoin(word_buff, segment);
					mask_buff = ft_strjoin(mask_buff, segment_mask);
				}
			}
		}
		if (word_buff)
			add_token(&tokens, create_token(TOKEN_WORD, word_buff, mask_buff));
	}
	print_tokens(tokens);
	return (tokens);
}
