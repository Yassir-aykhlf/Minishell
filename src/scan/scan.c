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

t_scan_status	ft_scan(const char *input)
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
