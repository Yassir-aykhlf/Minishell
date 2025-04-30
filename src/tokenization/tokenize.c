/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:20:36 by arajma            #+#    #+#             */
/*   Updated: 2025/04/30 21:32:49 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*extract_quoted_string(const char *input, int *pos, char quote,
		t_mask **mask)
{
	size_t	*segments;
	size_t	len;

	char *(str), *(mask_str);
	int (start) = *pos + 1;
	int (end) = start;
	while (input[end] && input[end] != quote)
		end++;
	len = end - start;
	str = ft_malloc(len + 1);
	ft_strlcpy(str, input + start, len + 1);
	mask_str = ft_malloc(len + 1);
	segments = ft_calloc(sizeof(size_t), len);
	ft_memset(mask_str, get_quote(quote), len);
	mask_str[len] = '\0';
	*mask = create_mask(mask_str, segments, len);
	if (input[end] == quote)
		*pos = end + 1;
	else
		*pos = end;
	return (str);
}

void	process_operator(const char *input, int *pos, t_token **tokens)
{
	int	i;

	i = 0;
	while (i < (int)(sizeof(s_operators) / sizeof(s_operators[0])))
	{
		if (ft_strncmp(input + *pos, s_operators[i].symbol,
				strlen(s_operators[i].symbol)) == 0)
		{
			add_operator_token(tokens, s_operators[i].type,
				s_operators[i].symbol, pos);
			return ;
			add_operator_token(tokens, s_operators[i].type,
				s_operators[i].symbol, pos);
			return ;
		}
		i++;
		i++;
	}
}

char	*process_word_segment(const char *input, int *pos, int len,
		t_mask **segment_mask)
{
	int (start);
	size_t(segment_len);
	char *(segment);
	if (input[*pos] == '\'' || input[*pos] == '\"')
		return (extract_quoted_string(input, pos, input[*pos], segment_mask));
	start = *pos;
	while (*pos < len && !is_whitespace(input[*pos]) && !ft_strchr("<>|&()'\"",
			input[*pos]))
		(*pos)++;
	segment_len = *pos - start;
	segment = ft_malloc(segment_len + 1);
	ft_strlcpy(segment, input + start, segment_len + 1);
	char *(mask_str) = ft_malloc(segment_len + 1);
	size_t *(segments) = ft_calloc(segment_len, sizeof(size_t));
	ft_memset(mask_str, 'N', segment_len);
	mask_str[segment_len] = '\0';
	*segment_mask = create_mask(mask_str, segments, segment_len);
	return (segment);
}

void	tokenizer_loop(const char *input, t_tokenizer_context *t)
{
	while (t->pos < t->len && !is_whitespace(input[t->pos])
		&& !ft_strchr("<>|&()", input[t->pos]))
	{
		t->segment_mask = NULL;
		t->segment = process_word_segment(input, &t->pos, t->len,
				&t->segment_mask);
		if (t->segment)
		{
			if (t->word_buff == NULL)
			{
				t->word_buff = t->segment;
				t->mask_buff = t->segment_mask;
			}
			else
			{
				t->temp_word = t->word_buff;
				t->temp_mask = t->mask_buff;
				t->word_buff = ft_strjoin(t->temp_word, t->segment);
				t->mask_buff = merge_masks(t->temp_mask, t->segment_mask);
			}
		}
	}
}

t_token	*ft_tokenize(const char *input)
{
	t_tokenizer_context	*t;

	t = init_tokenizer_context(input);
	while (t->pos < t->len)
	{
		skip_whitespace(input, &t->pos);
		if (t->pos >= t->len)
			break ;
		if (ft_strchr("<>|&()", input[t->pos]))
		{
			process_operator(input, &t->pos, &t->tokens);
			continue ;
		}
		t->word_buff = NULL;
		t->mask_buff = NULL;
		tokenizer_loop(input, t);
		if (t->word_buff)
			add_token(&t->tokens, create_token(TOKEN_WORD, t->word_buff,
					t->mask_buff));
	}
	return (t->tokens);
}
