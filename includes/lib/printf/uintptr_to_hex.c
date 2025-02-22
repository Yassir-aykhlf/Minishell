/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uintptr_to_hex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:15:55 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/15 14:17:19 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void	uintptr_to_hex(uintptr_t num, char *base, int *count)
{
	if (num >= 16)
		uintptr_to_hex(num / 16, base, count);
	print_char(base[num % 16], count);
}
