/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:58:51 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/13 14:42:35 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	err_exit(int status, char *message)
{
	if (message)
		perror(message);
	free_all();
	exit(status);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

bool	is_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	str = ft_malloc(n + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < n && s[i] != '\0')
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
