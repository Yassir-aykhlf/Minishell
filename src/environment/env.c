/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:02:28 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/17 17:37:55 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	**get_env_list(void)
{
	static t_env	*env_list;

	return (&env_list);
}

void	init_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp || !*envp)
		return ;
	while (envp[i])
	{
		parse_env_var(envp[i]);
		i++;
	}
}

char	*get_env_value(char *key)
{
	t_env	**env_list;
	t_env	*current;

	env_list = get_env_list();
	if (!env_list || !*env_list)
		return (NULL);
	current = *env_list;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	set_env(char *key, char *value)
{
	t_env	**env_list;
	t_env	*current;

	env_list = get_env_list();
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	link_env_node(env_list, create_env_node(key, value));
}

void	unset_env(char *key)
{
	t_env	**env_list;
	t_env	*current;

	env_list = get_env_list();
	current = *env_list;
	if (ft_strcmp(current->key, key) == 0)
		*env_list = current->next;
	while (current)
	{
		if (current->next && ft_strcmp(current->next->key, key) == 0)
			current->next = current->next->next;
		current = current->next;
	}
}

char	**env_to_array(void)
{
	t_env	**env_list;
	t_env	*current;
	char	**envp;
	int		size;
	int		i;

	i = 0;
	env_list = get_env_list();
	size = env_listsize(*env_list);
	envp = ft_malloc((size + 1) * sizeof(char *));
	current = *env_list;
	while (current)
	{
		envp[i] = ft_strjoin_three(current->key, "=", current->value);
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
