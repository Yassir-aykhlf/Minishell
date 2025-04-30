/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:58:00 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/30 21:33:08 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include <stddef.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE
}					t_token_type;

static const struct
{
	const char		*symbol;
	t_token_type	type;
} s_operators[] = {{"<<", TOKEN_HEREDOC}, {">>", TOKEN_APPEND}, {"&&",
	TOKEN_AND}, {"||", TOKEN_OR}, {"<", TOKEN_REDIRECT_IN}, {">",
	TOKEN_REDIRECT_OUT}, {"|", TOKEN_PIPE}, {"(", TOKEN_PAREN_OPEN}, {")",
	TOKEN_PAREN_CLOSE}};

typedef struct s_mask
{
	char			*mask;
	size_t			*seg;
	size_t			seg_size;
}					t_mask;

typedef struct s_token
{
	t_token_type	type;
	t_mask			*mask;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_tokenizer_context
{
	int				pos;
	int				len;
	char			*word_buff;
	t_mask			*mask_buff;
	char			*segment;
	t_mask			*segment_mask;
	char			*temp_word;
	t_mask			*temp_mask;
	t_token			*tokens;
}					t_tokenizer_context;

t_token				*ft_tokenize(const char *input);
void				tokenizer_loop(const char *input, t_tokenizer_context *t);
t_tokenizer_context	*init_tokenizer_context(const char *input);
t_mask				*merge_masks(t_mask *mask1, t_mask *mask2);
size_t				*create_merged_segments(const t_mask *mask1,
						const t_mask *mask2);
size_t				find_max_segment_id(const t_mask *mask, size_t length);
void				skip_whitespace(const char *input, int *pos);
char				*process_word_segment(const char *input, int *pos, int len,
						t_mask **segment_mask);
void				process_operator(const char *input, int *pos,
						t_token **tokens);
void				add_operator_token(t_token **tokens, t_token_type type,
						const char *value, int *pos);
char				*extract_quoted_string(const char *input, int *pos,
						char quote, t_mask **mask);
char				get_quote(char quote);
t_mask				*create_mask(char *mask_str, size_t *segments,
						size_t seg_count);
void				add_token(t_token **head, t_token *new_token);
t_token				*create_token(t_token_type type, char *value, t_mask *mask);

#endif
