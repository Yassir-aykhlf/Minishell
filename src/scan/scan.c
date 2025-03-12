/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:28:49 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/08 15:33:22 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	dual_operator(const char *input, int pos)
{
	if (input[pos] == '<')
	{
		if (input[pos + 1] == '<'
			&& ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	else if (input[pos] == '>')
	{
		if (input[pos + 1] == '>'
			&& ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	else if (input[pos] == '|')
	{
		if (input[pos + 1] == '|'
			&& ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	else if (input[pos] == '&')
	{
		if (input[pos + 1] == '&'
			&& ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
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

/**
 * @brief Scans the input string for syntax errors.
 * 
 * This function scans the input string to check for syntax errors such as
 * unclosed quotes or invalid operator sequences.
 * 
 * @param input The input string to be scanned.
 * @return t_scan_status The status of the scan, which can be one of the following:
 * - SCAN_SUCCESS: The input string has no syntax errors.
 * - SCAN_UNCLOSED_QUOTE: There is an unclosed quote in the input string.
 * - SCAN_INVALID_OPERATOR: There is an invalid operator sequence in the input string.
 */
t_scan_status	ft_scanner(const char *input)
{
	bool	in_single;
	bool	in_double;
	int		len;
	int		i;

	in_single = false;
	in_double = false;
	len = ft_strlen(input);
	i = 0;
	while (i < len)
	{
		if (!in_single && !in_double && is_operator_char(input[i]))
		{
			if (!valid_operator(input, i))
				return (SCAN_INVALID_OPERATOR);
			skip_operator(input, &i);
		}
		else
			handle_quotes(input[i], &in_single, &in_double);
		i++;
	}
	if (in_single || in_double)
		return (SCAN_UNCLOSED_QUOTE);
	return (SCAN_SUCCESS);
}

const char	*translate_message(t_scan_status status)
{
	if (status == SCAN_SUCCESS)
		return ("Syntax OK");
	else if (status == SCAN_UNCLOSED_QUOTE)
		return ("Error: Unclosed quote");
	else if (status == SCAN_INVALID_OPERATOR)
		return ("Error: Invalid operator sequence");
	else
		return ("Unknown error");
}
