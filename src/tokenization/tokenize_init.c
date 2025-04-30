/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 21:06:59 by arajma            #+#    #+#             */
/*   Updated: 2025/04/30 21:07:00 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_operator_token(t_token **tokens, t_token_type type,
		const char *value, int *pos)
{
	char	*operator_value;
	char	*mask_str;
	size_t	*segments;
	t_mask	*mask;
	size_t	len;

	len = ft_strlen(value);
	operator_value = ft_malloc(len + 1);
	ft_strlcpy(operator_value, value, len + 1);
	mask_str = ft_malloc(len + 1);
	ft_memset(mask_str, 'N', len);
	mask_str[len] = '\0';
	segments = ft_calloc(sizeof(size_t), len);
	mask = create_mask(mask_str, segments, len);
	add_token(tokens, create_token(type, operator_value, mask));
	*pos += len;
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

t_mask	*create_mask(char *mask_str, size_t *segments, size_t seg_count)
{
	t_mask	*mask;

	mask = ft_malloc(sizeof(t_mask));
	mask->mask = mask_str;
	mask->seg = segments;
	mask->seg_size = seg_count;
	return (mask);
}

t_token	*create_token(t_token_type type, char *value, t_mask *mask)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token));
	token->type = type;
	token->value = value;
	token->mask = mask;
	token->next = NULL;
	return (token);
}

t_tokenizer_context	*init_tokenizer_context(const char *input)
{
	t_tokenizer_context	*t;

	t = ft_malloc(sizeof(t_tokenizer_context));
	t->pos = 0;
	t->len = ft_strlen(input);
	t->tokens = NULL;
	t->temp_word = NULL;
	t->word_buff = NULL;
	t->mask_buff = NULL;
	return (t);
}
