/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 19:24:08 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 18:20:35 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	str = ft_malloc(n + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < n && s[i] != '\0')
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	syntax_error(t_ast *ast, t_token *tokens)
{
	if (!ast || tokens)
	{
		if (tokens)
			syntax_print(tokens->value);
		else
			syntax_print("syntax error");
		return (1);
	}
	return (0);
}

bool	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9')
		|| c == '_');
}

char	*last_token(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current->next)
		current = current->next;
	return (current->value);
}
