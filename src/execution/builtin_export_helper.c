/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:35:29 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 17:35:37 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parse_and_set_export_var(char *arg)
{
	char	*key;
	char	*value;
	char	*equals_pos;

	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
	{
		key = ft_strndup(arg, equals_pos - arg);
		if (!key)
		{
			spit_error(EXIT_FAILURE, "ft_strndup", true);
			return;
		}
		value = equals_pos + 1;
		set_env_var(key, value);
	}
	else
	{
		key = arg;
		set_env_var(key, NULL);
	}
}
