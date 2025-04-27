/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:08:31 by arajma            #+#    #+#             */
/*   Updated: 2025/04/27 21:15:09 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expand_args_list(t_args **args_head)
{
	t_args *(current) = *args_head;
	t_args *(prev) = NULL;
	int (field_sp) = is_field_split(*args_head);
	while (current)
	{
		t_args (*next) = current->next;
		if (current->arg && current->mask)
		{
			t_args (*expanded) = expand_token(current->arg,
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
	t_args *(expanded);
	t_redir *(redir) = redirects;
	while (redir)
	{
		if (redir->file && redir->mask)
		{
			expanded = expand_token(redir->file, redir->mask, 1);
			redir->file = ft_strdup(expanded->arg);
			redir->mask = ft_strdup(expanded->mask);
		}
		redir = redir->next;
	}
}

t_args	*expand_token(char *token, char *mask, int fs)
{
	t_expand *(ex) = init_exp_cntext(token, mask, fs);
	while (token[ex->pos])
	{
		if (token[ex->pos] == '$' && mask[ex->pos] != 'S')
			handle_var(ex);
		else
		{
			ex->word = append_char(ex->word, token[ex->pos]);
			ex->pos++;
		}
	}
	if (ex->word && *(ex->word))
		add_to_list(ex);
	return (*(ex->head));
}

int	handle_field_splitting(t_expand *ex, const char *value, int var_start)
{
	int (i) = 1;
	char *(tmp);
	if (ex->mask[var_start] != 'N'
		|| !contains_whitespace(value) || ex->fs != 0)
		return (0);
	char **(words) = ft_split_whitespace(value);
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
	char *(tmp);
	int (var_start) = ex->pos++;
	if (ex->token[ex->pos] == '?')
	{
		handle_exit_code(ex);
		return ;
	}
	char *(name) = get_var_name(ex);
	if (!name)
		return ;
	char *(value) = get_var_value(name);
	if (!handle_field_splitting(ex, value, var_start))
	{
		tmp = ft_strjoin(ex->word, value);
		ex->word = tmp;
	}
}
