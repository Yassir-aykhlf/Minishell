/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:37:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/21 18:05:58 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	if (!key || !value)
		return (NULL);
	node = ft_calloc(1, sizeof(t_env));
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

void	link_env_node(t_env **head, t_env *node)
{
	t_env	*current;

	if (!head || !node)
		return ;
	if (!*head)
	{
		*head = node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = node;
}

void	parse_env_var(const char *var)
{
	char	*key;
	char	*pos;
	char	*value;
	t_env	**env_list;

	env_list = get_env_list();
	pos = ft_strchr(var, '=');
	key = ft_strndup(var, pos - var);
	value = ft_strdup(pos + 1);
	link_env_node(env_list, create_env_node(key, value));
}
