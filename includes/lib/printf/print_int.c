/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:24:07 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/15 14:24:21 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void	print_int(int num, int *count)
{
	if (num == -2147483648)
	{
		print_str("-2147483648", count);
		return ;
	}
	if (num < 0)
	{
		print_char('-', count);
		num = -num;
	}
	if (num >= 10)
	{
		print_int(num / 10, count);
		print_int(num % 10, count);
	}
	else
		print_char(num + '0', count);
}
