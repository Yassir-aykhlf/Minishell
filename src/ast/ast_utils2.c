/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:41:34 by arajma            #+#    #+#             */
/*   Updated: 2025/03/14 20:59:09 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Function to handle redirections for any node type */
t_ast	*handle_redirections(t_ast *node, t_token **tokens)
{
	t_token_type	redirect_type;
	t_token			*current;

	while (*tokens)
	{
		current = *tokens;
		if (!is_redirect(current))
			break ;
		redirect_type = current->type;
		*tokens = current->next;
		if (!*tokens || ((*tokens)->type != TOKEN_WORD))
			return (NULL);
		add_redirect(node, redirect_type, (*tokens)->value);
		*tokens = (*tokens)->next;
	}
	return (node);
}

/* Main parsing function */
t_ast	*ft_parse(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (parse_logical_expr(tokens));
}

/* C'mon it is just here for normes */
int	is_redirect(t_token *token)
{
	if (token->type == TOKEN_REDIRECT_IN
		|| token->type == TOKEN_REDIRECT_OUT
		|| token->type == TOKEN_APPEND
		|| token->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

/* C'mon it is just here for normes */
int	is_cmd_finished(t_token *token)
{
	if (token->type == TOKEN_PIPE
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_OR)
		return (1);
	return (0);
}
