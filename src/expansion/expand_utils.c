/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:14:09 by arajma            #+#    #+#             */
/*   Updated: 2025/04/28 11:32:16 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expand_command(t_ast *cmd)
{
	if (!cmd || cmd->type != NODE_COMMAND)
		return ;
	if (cmd->u_data.s_cmd.argv)
		expand_args_list(&cmd->u_data.s_cmd.argv);
	if (cmd->u_data.s_cmd.redirects)
		expand_redirections(cmd->u_data.s_cmd.redirects);
}

void	add_to_list(t_expand *ex)
{
	int (i) = 0;
	t_args *(temp), *(new_arg) = ft_malloc(sizeof(t_args));
	new_arg->arg = ft_strdup(ex->word);
	new_arg->mask = ft_calloc(1, 1);
	new_arg->next = NULL;
	if (!*(ex->head))
		*(ex->head) = new_arg;
	else
	{
		temp = *(ex->head);
		while (temp->next)
			temp = temp->next;
		temp->next = new_arg;
	}
	ex->word = ft_strdup("");
}

void	handle_exit_code(t_expand *ex)
{
	ex->pos++;
	char *(exit_code) = ft_itoa(*get_exit_status());
	char *(tmp) = ft_strjoin(ex->word, exit_code);
	ex->word = tmp;
}

char	**ft_split_whitespace(const char *s)
{
	if (!s)
		return (NULL);
	char *(normalized) = ft_strdup(s);
	int (i) = 0;
	while (normalized[i])
	{
		if (ft_isspace(normalized[i]))
			normalized[i] = ' ';
		i++;
	}
	char **(result) = ft_split(normalized, ' ');
	return (result);
}
