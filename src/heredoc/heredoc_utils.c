/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:46:59 by arajma            #+#    #+#             */
/*   Updated: 2025/05/03 16:56:07 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_var_name_hd(char *str, char **var_name)
{
	int		i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '?')
	{
		*var_name = ft_strdup("?");
		return (1);
	}
	while (str[i] && is_valid_var_char(str[i]))
		i++;
	if (i == 0)
		return (0);
	*var_name = ft_strndup(str, i);
	return (i);
}

char	*get_var_value_hd(char *var_name)
{
	if (!var_name || !var_name[0])
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(*get_exit_status()));
	return (get_var_value(var_name));
}

char	*expand_heredoc(char *input)
{
	int	(var_len), (i) = 0;
	char *(var_value), *(var_name), *(result) = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1])
		{
			var_len = get_var_name_hd(&input[i + 1], &var_name);
			if (var_len > 0)
			{
				var_value = get_var_value_hd(var_name);
				result = ft_strjoin(result, var_value);
				i += var_len + 1;
			}
			else
				result = append_char(result, input[i++]);
		}
		else
			result = append_char(result, input[i++]);
	}
	return (result);
}

int	to_expand(char *mask)
{
	int	i;

	i = 0;
	if (!mask)
		return (0);
	while (mask[i])
		if (mask[i++] != 'N')
			return (0);
	return (1);
}
