/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:30:59 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 17:34:25 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '_');
}

int	is_valid_identifier(char *key, char *full_arg)
{
	int	j;

	if (!((key[0] >= 'a' && key[0] <= 'z') ||
		(key[0] >= 'A' && key[0] <= 'Z') || key[0] == '_'))
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(full_arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	j = 1;
	while (key[j])
	{
		if (!is_valid_var_char(key[j]))
		{
			ft_putstr_fd("export: `", STDERR_FILENO);
			ft_putstr_fd(full_arg, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		j++;
	}
	return (0);
}

int	validate_export(char **args)
{
	int		i;
	char	*key;
	char	*equals_pos;
	int		status;

	if (!args)
		return (EXIT_FAILURE);
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!args[i][0])
		{
			i++;
			continue;
		}
		equals_pos = ft_strchr(args[i], '=');
		if (equals_pos)
		{
			key = ft_strndup(args[i], equals_pos - args[i]);
			if (!key) return (spit_error(EXIT_FAILURE, "ft_strndup", true));
		}
		else
			key = args[i];
		if (is_valid_identifier(key, args[i]) != 0)
			status = EXIT_FAILURE;
		i++;
	}
	return (status);
}

void	builtin_export_print(char **env)
{
	t_env	**env_list_head;
	t_env	*current;

	(void)env;
	env_list_head = get_env_list();
	if (!env_list_head)
		return;
	current = *env_list_head;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

int	builtin_export(char **args, char **env)
{
	int i;

	(void)env;
	if (validate_export(args) != 0)
		return (EXIT_FAILURE);
	if (!args[1])
	{
		builtin_export_print(NULL);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (args[i][0])
			parse_and_set_export_var(args[i]);
		i++;
	}
	return (0);
}
