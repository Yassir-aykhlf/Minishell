/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:58:51 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 19:25:44 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_listsize(t_env *lst)
{
	int		count;
	t_env	*current;

	if (!lst)
		return (0);
	count = 0;
	current = lst;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	char	*str;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	str = ft_malloc(len1 + len2 + len3 + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, len1);
	ft_memcpy(str + len1, s2, len2);
	ft_memcpy(str + len1 + len2, s3, len3);
	str[len1 + len2 + len3] = '\0';
	return (str);
}

int	spit_error(int status, char *message, bool system_error)
{
	if (message)
	{
		if (system_error)
			perror(message);
		else
			write(2, message, ft_strlen(message));
	}
	return (status);
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
