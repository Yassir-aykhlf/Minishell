/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:25:30 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/17 19:43:23 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static void	conversion(char *s, va_list args, int *count)
{
	if (*(s + 1) == 'c')
		print_char((char)va_arg(args, int), count);
	else if (*(s + 1) == 's')
		print_str(va_arg(args, char *), count);
	else if (*(s + 1) == 'x')
		print_hex_low((unsigned int)va_arg(args, int), count);
	else if (*(s + 1) == 'X')
		print_hex_upp((unsigned int)va_arg(args, int), count);
	else if (*(s + 1) == 'p')
		print_ptr((uintptr_t)va_arg(args, void *), count);
	else if (*(s + 1) == '%')
		print_char('%', count);
	else if (*(s + 1) == 'i' || *(s + 1) == 'd')
		print_int(va_arg(args, int), count);
	else if (*(s + 1) == 'u')
		print_uint(va_arg(args, unsigned int), count);
	else
	{
		print_char('%', count);
		print_char(*(s + 1), count);
	}
}

int	ft_printf(const char *s, ...)
{
	int		count;
	va_list	args;

	count = 0;
	if (!s)
		return (-1);
	va_start(args, s);
	while (*s)
	{
		if (*s == '%')
		{
			if (!(*(s + 1)))
			{
				va_end(args);
				return (-1);
			}
			conversion((char *)(s), args, &count);
			s++;
		}
		else
			print_char(*s, &count);
		s++;
	}
	va_end(args);
	return (count);
}
