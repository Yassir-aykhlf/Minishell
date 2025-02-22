/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:21:03 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/16 11:08:13 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void	print_ptr(uintptr_t address, int *count)
{
	if (address == 0)
	{
		print_char('(', count);
		print_char('n', count);
		print_char('i', count);
		print_char('l', count);
		print_char(')', count);
		return ;
	}
	print_char('0', count);
	print_char('x', count);
	uintptr_to_hex(address, "0123456789abcdef", count);
}
