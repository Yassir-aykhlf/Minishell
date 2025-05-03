/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:05:05 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 17:06:15 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_dual_operator_sequence(const char *input, int pos, char op)
{
	if (input[pos + 1] == op && ft_strchr("<>|&", input[pos + 2]))
		return (false);
	return (true);
}

bool	dual_operator(const char *input, int pos)
{
	if (input[pos] == '<')
		return (check_dual_operator_sequence(input, pos, '<'));
	else if (input[pos] == '>')
		return (check_dual_operator_sequence(input, pos, '>'));
	else if (input[pos] == '|')
		return (check_dual_operator_sequence(input, pos, '|'));
	else if (input[pos] == '&')
		return (check_dual_operator_sequence(input, pos, '&'));
	return (true);
}

bool	valid_operator(const char *input, int pos)
{
	if (input[pos] == '<' && ft_strchr(">|&", input[pos + 1]))
		return (false);
	else if (input[pos] == '>' && ft_strchr("<|&", input[pos + 1]))
		return (false);
	else if (input[pos] == '|' && ft_strchr("<>&", input[pos + 1]))
		return (false);
	else if (input[pos] == '&' && ft_strchr("<>|", input[pos + 1]))
		return (false);
	return (dual_operator(input, pos));
}

void	handle_quotes(char c, bool *in_single, bool *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '\"' && !(*in_single))
		*in_double = !(*in_double);
}

void	skip_operator(const char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
		(*i)++;
	else if (input[*i] == '>' && input[*i + 1] == '>')
		(*i)++;
	else if (input[*i] == '|' && input[*i + 1] == '|')
		(*i)++;
	else if (input[*i] == '&' && input[*i + 1] == '&')
		(*i)++;
}
