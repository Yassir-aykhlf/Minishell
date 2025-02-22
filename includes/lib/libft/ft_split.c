/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:57:46 by yaykhlf           #+#    #+#             */
/*   Updated: 2024/11/09 16:52:41 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *str, char c)
{
	size_t	i;
	size_t	count;
	int		in_word;

	if (!str)
		return (0);
	i = 0;
	count = 0;
	in_word = 0;
	while (str[i])
	{
		if (str[i] != c && !in_word)
		{
			count++;
			in_word = 1;
		}
		else if (str[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

static size_t	word_len(const char *s, char c)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static void	free_array(char **array, size_t	len)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (i < len)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static char	**split(const char *s, char c, size_t words, char **result)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (*s && i < words)
	{
		while (*s && *s == c)
			s++;
		len = word_len(s, c);
		result[i] = (char *)malloc(sizeof(char) * (len + 1));
		if (!result[i])
		{
			free_array(result, i);
			return (NULL);
		}
		ft_strlcpy(result[i], s, len + 1);
		s += len;
		i++;
	}
	result[i] = NULL;
	return (result);
}

char	**ft_split(const char *s, char c)
{
	size_t	words;
	char	**result;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	result = (char **)malloc(sizeof(char *) * (words + 1));
	if (!result)
		return (NULL);
	return (split(s, c, words, result));
}
