/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:21:40 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/15 14:22:35 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void	print_str(char *str, int *count)
{
	if (!str)
	{
		print_str("(null)", count);
		return ;
	}
	while (*str)
	{
		print_char(*str, count);
		str++;
	}
}
