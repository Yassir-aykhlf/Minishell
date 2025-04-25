/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:25:53 by arajma            #+#    #+#             */
/*   Updated: 2025/04/25 15:26:06 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_sstat(t_expand *ex, int start)
{
	if (start)
		return (ex->mask[ex->pos] == ex->mask[start - 1]);
	return (ex->mask[ex->pos] == ex->mask[start]);
}

int	is_alnumus(char c)
{
	return ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9')
		|| c == '_');
}

int	is_field_split(t_args *arg)
{
	char *(str) = arg->mask;
	int (i) = 0;
	if (ft_strcmp(arg->arg, "export"))
		return (0);
	while (str && str[i])
	{
		if (str[i] != 'N')
			return (0);
		i++;
	}
	return (1);
}

t_args	*add_expanded_nodes(t_args *expanded, t_args *next,
					t_args **args_head, t_args *prev)
{
	if (!expanded)
		return (prev);
	t_args *(last) = expanded;
	while (last->next)
		last = last->next;
	last->next = next;
	if (prev)
		prev->next = expanded;
	else
		*args_head = expanded;
	return (last);
}
