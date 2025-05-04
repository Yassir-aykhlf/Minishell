/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:38:10 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 15:04:55 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_valid_number(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+')
		i++;
	if (arg[i] == '-' && ft_isdigit((unsigned char)arg[i + 1]))
		i++;
	while (arg[i])
	{
		if (!ft_isdigit((unsigned char)arg[i]))
			return (false);
		i++;
	}
	return (true);
}

int	parse_exit_status(char *arg, int *status)
{
	if (!arg)
	{
		*status = *get_exit_status();
		return (0);
	}
	if (is_valid_number(arg))
	{
		*status = ft_atoi(arg);
		return (0);
	}
	else
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		*status = 2;
		return (2);
	}
}

int	builtin_exit(char **args)
{
	int	status;
	int	parse_result;
	int	arg_count;

	arg_count = 0;
	while (args && args[arg_count])
		arg_count++;
	if (!is_child_process())
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	parse_result = parse_exit_status(args[1], &status);
	if (parse_result != 0)
		exit(status & 0xFF);
	if (arg_count > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	exit(status & 0xFF);
	return (EXIT_FAILURE);
}
