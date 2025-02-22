/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:07:44 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/05 15:13:17 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	sizeofnum(long l)
{
	size_t	size;

	size = 0;
	while (l / 10 > 0)
	{
		size++;
		l = l / 10;
	}
	return (size + 1);
}

static	char	*reverse(char *str)
{
	size_t	len;
	size_t	i;
	char	temp;

	i = 0;
	len = ft_strlen(str);
	while (i < len / 2)
	{
		temp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = temp;
		i++;
	}
	return (str);
}

static	void	fill_number(char *str, long l, int *i)
{
	if (l == 0)
	{
		str[*i] = '0';
		(*i)++;
		return ;
	}
	while (l > 0)
	{
		str[*i] = (l % 10) + '0';
		l = l / 10;
		(*i)++;
	}
}

char	*ft_itoa(int n)
{
	size_t	size;
	int		i;
	long	l;
	char	*str;

	size = 0;
	i = 0;
	l = (long)n;
	if (l < 0)
	{
		size = 1;
		l = -l;
	}
	size += sizeofnum(l);
	str = (char *)malloc((size + 1) * sizeof(char));
	if (!str)
		return (NULL);
	fill_number(str, l, &i);
	if (n < 0)
		str[i++] = '-';
	str[i] = '\0';
	return (reverse(str));
}
