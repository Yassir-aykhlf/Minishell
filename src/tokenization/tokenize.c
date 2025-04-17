/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:01:14 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/17 15:55:12 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value, char *mask) {
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

void	add_token(t_token **head, t_token *new_token) {
	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	t_token	*current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

char	*extract_quoted_string(const char *input, int *pos, char quote, char **mask) {
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
	ft_memset(local_mask, (quote == '\'') ? 'S' : 'D', end - start);
	*mask = local_mask;
	*pos = (input[end] == quote) ? end + 1 : end;
	return (str);
}

void	add_operator_token(t_token **tokens, t_token_type type, const char *value, int *pos) {
	char *mask = ft_calloc(ft_strlen(value) + 1, sizeof(char));
	if (mask)
		ft_memset(mask, 'N', ft_strlen(value));
	add_token(tokens, create_token(type, ft_strdup(value), mask));
	*pos += ft_strlen(value);
}

void	process_operator(const char *input, int *pos, t_token **tokens) {
	static const struct {
		const char	*symbol;
		t_token_type	type;
	} operators[] = {
		{"<<", TOKEN_HEREDOC}, {">>", TOKEN_APPEND}, {"&&", TOKEN_AND},
		{"||", TOKEN_OR}, {"<", TOKEN_REDIRECT_IN}, {">", TOKEN_REDIRECT_OUT},
		{"|", TOKEN_PIPE}, {"(", TOKEN_PAREN_OPEN}, {")", TOKEN_PAREN_CLOSE}
	};
	for (int i = 0; i < (int)(sizeof(operators) / sizeof(operators[0])); i++)
	{
		if (strncmp(input + *pos, operators[i].symbol, ft_strlen(operators[i].symbol)) == 0)
		{
			add_operator_token(tokens, operators[i].type, operators[i].symbol, pos);
			return;
		}
	}
}

char	*process_word_segment(const char *input, int *pos, int len, char **segment_mask) {
	int		start;
	char	quote;
	char	*segment = NULL;

	if (input[*pos] == '\'' || input[*pos] == '\"') {
		quote = input[*pos];
		segment = extract_quoted_string(input, pos, quote, segment_mask);
	} else {
		start = *pos;
		while (*pos < len && !is_whitespace(input[*pos]) && 
				!ft_strchr("<>|&()'\"", input[*pos]))
			(*pos)++;
		segment = ft_strndup(input + start, *pos - start);
		*segment_mask = ft_calloc(*pos - start + 1, sizeof(char));
		if (*segment_mask)
			ft_memset(*segment_mask, 'N', *pos - start);
	}
	return segment;
}

void	skip_whitespace(const char *input, int *pos) {
	while (input[*pos] && is_whitespace(input[*pos]))
		(*pos)++;
}

void	print_tokens(t_token *tokens) {
	t_token	*current = tokens;

	while (current)
	{
		printf("Token: %s, Mask: %s\n", current->value, current->mask);
		current = current->next;
	}
}

t_token	*ft_tokenize(const char *input) {
	int		pos;
	int		len;
	char	*word_buffer = NULL;
	char	*mask_buffer = NULL;
	char	*segment;
	char	*segment_mask;
	t_token	*tokens;

	pos = 0;
	len = ft_strlen(input);
	tokens = NULL;
	while (pos < len) {
		skip_whitespace(input, &pos);
		if (pos >= len)
			break;
		if (ft_strchr("<>|&()", input[pos])) {
			process_operator(input, &pos, &tokens);
			continue;
		}
		word_buffer = NULL;
		mask_buffer = NULL;
		
		while (pos < len && !is_whitespace(input[pos]) && 
			   !ft_strchr("<>|&()", input[pos])) {
			segment_mask = NULL;
			segment = process_word_segment(input, &pos, len, &segment_mask);
			
			if (segment) {
				if (word_buffer == NULL)
				{
					word_buffer = segment;
					mask_buffer = segment_mask;
				}
				else {
					char *temp_word = word_buffer;
					char *temp_mask = mask_buffer;
					
					word_buffer = ft_strjoin(word_buffer, segment);
					mask_buffer = ft_strjoin(mask_buffer, segment_mask);
				}
			}
		}
		if (word_buffer) {
			add_token(&tokens, create_token(TOKEN_WORD, word_buffer, mask_buffer));
		}
	}
	print_tokens(tokens);
	return (tokens);
}
