/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:35:29 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 14:37:16 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_append_operation(char *key, char *value)
{
	char	*existing_value;
	char	*new_value;

	existing_value = get_env_value(key);
	if (existing_value)
	{
		new_value = ft_strjoin(existing_value, value);
		set_env_var(key, new_value);
	}
	else
		set_env_var(key, value);
}

static void	process_append_var(char *arg, char *append_pos)
{
	char	*key;
	char	*value;

	key = ft_strndup(arg, append_pos - arg);
	if (!key)
	{
		spit_error(EXIT_FAILURE, "ft_strndup", true);
		return ;
	}
	value = append_pos + 2;
	handle_append_operation(key, value);
}

static void	process_equals_var(char *arg, char *equals_pos)
{
	char	*key;
	char	*value;

	if (equals_pos)
	{
		key = ft_strndup(arg, equals_pos - arg);
		if (!key)
		{
			spit_error(EXIT_FAILURE, "ft_strndup", true);
			return ;
		}
		value = equals_pos + 1;
		set_env_var(key, value);
	}
	else
		set_env_var(arg, NULL);
}

void	parse_and_set_export_var(char *arg)
{
	char	*append_pos;
	char	*equals_pos;

	append_pos = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (append_pos && append_pos[2] != '\0')
		process_append_var(arg, append_pos);
	else
	{
		equals_pos = ft_strchr(arg, '=');
		process_equals_var(arg, equals_pos);
	}
}
