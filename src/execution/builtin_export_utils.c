/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:38:45 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 14:39:00 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_export_append(char *arg, char *append_pos)
{
	char	*key;
	int		status;

	key = ft_strndup(arg, append_pos - arg);
	if (!key)
		return (spit_error(EXIT_FAILURE, "ft_strndup", true));
	status = is_valid_identifier(key, arg);
	return (status);
}

int	validate_export_equals(char *arg, char *equals_pos)
{
	char	*key;
	int		status;

	if (equals_pos)
	{
		key = ft_strndup(arg, equals_pos - arg);
		if (!key)
			return (spit_error(EXIT_FAILURE, "ft_strndup", true));
		status = is_valid_identifier(key, arg);
	}
	else
	{
		key = arg;
		status = is_valid_identifier(key, arg);
	}
	return (status);
}

int	validate_single_export_arg(char *arg)
{
	char	*equals_pos;
	char	*append_pos;

	if (!arg || !arg[0])
		return (0);
	append_pos = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (append_pos && append_pos[2] != '\0')
		return (validate_export_append(arg, append_pos));
	equals_pos = ft_strchr(arg, '=');
	return (validate_export_equals(arg, equals_pos));
}
