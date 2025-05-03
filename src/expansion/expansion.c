/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:08:31 by arajma            #+#    #+#             */
/*   Updated: 2025/05/03 16:51:34 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expand_args_list(t_args **args_head)
{
	t_args	*expanded;
	t_args	*current;
	t_args	*prev;
	t_args	*next;
	int		field_sp;

	current = *args_head;
	prev = NULL;
	field_sp = is_field_split(*args_head);
	while (current)
	{
		next = current->next;
		if (current->arg && current->mask)
		{
			expanded = expand_token(current->arg,
				current->mask, field_sp);
			prev = add_expanded_nodes(expanded, next, args_head, prev);
		}
		else
			prev = current;
		current = next;
	}
}

void	expand_redirections(t_redir *redirects)
{
	t_args	*expanded;
	t_redir	*redir;

	redir = redirects;
	while (redir)
	{
		if (redir->file && redir->mask)
		{
			expanded = expand_token(redir->file, redir->mask, 1);
			redir->file = ft_strdup(expanded->arg);
			redir->mask = expanded->mask;
		}
		redir = redir->next;
	}
}

t_args	*expand_token(char *token, t_mask *mask, int fs)
{
	t_expand	*ex;

	ex = init_exp_cntext(token, mask, fs);
	while (token[ex->pos])
	{
		if (token[ex->pos] == '$' && mask->mask[ex->pos] != 'S')
			handle_var(ex);
		else
		{
			ex->word = append_char(ex->word, token[ex->pos]);
			ex->pos++;
		}
	}
	if (ex->word)
		add_to_list(ex);
	return (*(ex->head));
}

int	handle_field_splitting(t_expand *ex, const char *value, int var_start)
{
	int		i;
	char	*tmp;
	char	**words;

	i = 1;
	if (ex->mask->mask[var_start] != 'N'
		|| !contains_whitespace(value) || ex->fs != 0)
		return (0);
	words = ft_split_whitespace(value);
	if (words && words[0])
	{
		tmp = ft_strjoin(ex->word, words[0]);
		ex->word = tmp;
	}
	add_to_list(ex);
	while (words && words[i])
	{
		ex->word = ft_strdup(words[i++]);
		add_to_list(ex);
	}
	return (1);
}

void	handle_var(t_expand *ex)
{
	char	*tmp;
	char	*name;
	char	*value;
	int		var_start;

	var_start = ex->pos++;
	if (ex->token[ex->pos] == '?')
	{
		handle_exit_code(ex);
		return ;
	}
	name = get_var_name(ex);
	if (!name)
		return ;
	value = get_var_value(name);
	if (!handle_field_splitting(ex, value, var_start))
	{
		tmp = ft_strjoin(ex->word, value);
		ex->word = tmp;
	}
}
