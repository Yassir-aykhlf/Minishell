/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:16:20 by arajma            #+#    #+#             */
/*   Updated: 2025/05/04 20:22:10 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_var_name(t_expand *ex)
{
	int	start;

	start = ex->pos;
	while (ex->token[ex->pos]
		&& is_sstat(ex, start)
		&& ex->mask->seg[start - 1] == ex->mask->seg[ex->pos]
		&& is_valid_var_char(ex->token[ex->pos]))
		ex->pos++;
	if (ex->pos == start)
	{
		if (ex->token[ex->pos] == '\0')
			ex->word = ft_strjoin(ex->word, "$");
		return (NULL);
	}
	return (ft_strndup(ex->token + start, ex->pos - start));
}

char	*get_var_value(char *name)
{
	t_env	**env_list;
	t_env	*env;

	env_list = get_env_list();
	env = *env_list;
	if (name && (name[0] <= '9' && name[0] >= '0'))
		return (ft_strdup(name + 1));
	while (env)
	{
		if (ft_strcmp(env->key, name) == 0)
		{
			if (env->value)
				return (ft_strdup(env->value));
			else
				return (ft_strdup(""));
		}
		env = env->next;
	}
	return (ft_strdup(""));
}

t_expand	*init_exp_cntext(char *token, t_mask *mask, int fs)
{
	t_expand	*new;

	new = ft_malloc(sizeof(t_expand));
	new->head = ft_malloc(sizeof(t_args *));
	*(new->head) = NULL;
	new->pos = 0;
	new->fs = fs;
	new->token = token;
	new->mask = mask;
	new->word = ft_strdup("");
	return (new);
}

char	*append_char(char *str, char c)
{
	char	*append;

	append = ft_malloc(2);
	append[0] = c;
	append[1] = '\0';
	return (ft_strjoin(str, append));
}

int	contains_whitespace(const char *str)
{
	while (*str)
	{
		if (ft_isspace((unsigned char)*str))
			return (1);
		str++;
	}
	return (0);
}
