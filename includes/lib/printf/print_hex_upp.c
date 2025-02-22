/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex_upp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:19:43 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/15 14:20:01 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void	print_hex_upp(unsigned int decimal, int *count)
{
	if (decimal >= 16)
		print_hex_upp(decimal / 16, count);
	print_char("0123456789ABCDEF"[decimal % 16], count);
}
