/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_uint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:24:44 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/15 14:25:05 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void	print_uint(unsigned int num, int *count)
{
	if (num >= 10)
	{
		print_uint(num / 10, count);
		print_uint(num % 10, count);
	}
	else
		print_char(num + '0', count);
}
