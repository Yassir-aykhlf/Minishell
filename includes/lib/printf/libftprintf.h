/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:18:28 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/16 10:04:15 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <unistd.h>
# include <stddef.h>

int		ft_printf(const char *str, ...);
void	print_char(char c, int *count);
void	print_int(int num, int *count);
void	print_str(char *str, int *count);
void	print_ptr(uintptr_t address, int *count);
void	print_uint(unsigned int num, int *count);
void	print_hex_low(unsigned int decimal, int *count);
void	print_hex_upp(unsigned int decimal, int *count);
void	uintptr_to_hex(uintptr_t num, char *base, int *count);

#endif
