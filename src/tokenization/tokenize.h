/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:58:00 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/08 18:19:19 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

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
	TOKEN_PAREN_CLOSE,
	TOKEN_SINGLE_QUOTED,
	TOKEN_DOUBLE_QUOTED
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

char	*extract_quoted_string(const char *input, int *pos, char quote);
void	handle_operator(const char *input, int *pos, t_token **tokens);
void	add_token(t_token **head, t_token *new_token);
t_token	*create_token(t_token_type type, char *value);
t_token	*ft_tokenize(const char *input);
void	free_tokens(t_token *tokens);

#endif
