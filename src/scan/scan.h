/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:05:34 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/05 15:51:16 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAN_H
# define SCAN_H

# include <stdbool.h>
# include <string.h>
# include <stdio.h>

typedef enum e_scan_status
{
	SCAN_SUCCESS,
	SCAN_UNCLOSED_PAREN,
	SCAN_UNCLOSED_QUOTE,
	SCAN_INVALID_OPERATOR
}	t_scan_status;

void			handle_quotes(char c, bool *in_single, bool *in_double);
bool			valid_operator(const char *input, int pos);
bool			dual_operator(const char *input, int pos);
const char		*translate_message(t_scan_status status);
void			skip_operator(const char *input, int *i);
t_scan_status	ft_scan(const char *input);
bool			is_operator_char(char c);
bool			check_dual_operator_sequence(const char *input, int pos,
					char op);

#endif
