/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex_low.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:20:20 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/15 14:20:39 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void	print_hex_low(unsigned int decimal, int *count)
{
	if (decimal >= 16)
		print_hex_low(decimal / 16, count);
	print_char("0123456789abcdef"[decimal % 16], count);
}
