/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:02:28 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/30 22:07:08 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	**get_env_list(void)
{
	static t_env	*env_list;

	return (&env_list);
}

void	init_env_ll(char **env)
{
	int	i;

	i = 0;
	if (!env || !*env)
		return ;
	while (env[i])
	{
		parse_env_var(env[i]);
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
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	set_env_var(char *key, char *value)
{
	t_env	**env_list;
	t_env	*current;

	env_list = get_env_list();
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (value)
				current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	link_env_node(env_list, create_env_node(key, value));
}

void	unset_env_var(char *key)
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

void	print_env(void)
{
	t_env	**env_list;
	t_env	*current;
	int		i;

	i = 0;
	env_list = get_env_list();
	current = *env_list;
	while (current)
	{
		i++;
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		else
			printf("%s\n", current->key);
		current = current->next;
	}
	return ;
}

char	**env_to_array(void)
{
	t_env	**env_list;
	t_env	*current;
	char	**env;
	int		size;
	int		i;

	i = 0;
	env_list = get_env_list();
	size = env_listsize(*env_list);
	env = ft_malloc((size + 1) * sizeof(char *));
	current = *env_list;
	while (current)
	{
		env[i] = ft_strjoin_three(current->key, "=", current->value);
		current = current->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}
