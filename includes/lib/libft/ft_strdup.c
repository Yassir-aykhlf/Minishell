/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:36:21 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/08 16:20:41 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*cpy;
	size_t	len;

	len = ft_strlen(s);
	cpy = (char *)ft_malloc(len + 1);
	if (!cpy)
		return (NULL);
	ft_memcpy (cpy, s, len + 1);
	return (cpy);
}
