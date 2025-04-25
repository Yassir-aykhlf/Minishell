/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:16:20 by arajma            #+#    #+#             */
/*   Updated: 2025/04/25 15:42:05 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_var_name(t_expand *ex)
{
	char *(tmp);
	int (start) = ex->pos;
	while (ex->token[ex->pos]
		&& is_sstat(ex, start)
		&& is_alnumus(ex->token[ex->pos]))
		ex->pos++;
	if (ex->pos == start)
	{
		tmp = ft_strjoin(ex->word, "$");
		ex->word = tmp;
		return (NULL);
	}
	return (ft_strndup(ex->token + start, ex->pos - start));
}

char	*get_var_value(char *name)
{
	t_env **(env_list) = get_env_list();
	t_env *(env) = *env_list;
	while (env)
	{
		if (ft_strcmp(env->key, name) == 0)
			return (env->value);
		env = env->next;
	}
	return (ft_strdup(""));
}

t_expand	*init_exp_cntext(char *token, char *mask, int fs)
{
	t_expand *(new) = ft_malloc(sizeof(t_expand));
	new->head = ft_malloc(8);
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
