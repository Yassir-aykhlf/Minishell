/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:38:10 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 17:40:04 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_exit_status(char *arg, int *status)
{
	if (arg)
	{
		if (ft_isdigit((unsigned char)arg[0]) ||
			(arg[0] == '-' && ft_isdigit((unsigned char)arg[1])))
		{
			*status = ft_atoi(arg);
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
	else
		*status = 0;
	return (0);
}

int	builtin_exit(char **args)
{
	int	status;
	int	parse_result;

	parse_result = parse_exit_status(args[1], &status);
	if (parse_result != 0)
		return (parse_result);
	if (!is_child_process())
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(status & 0xFF);
	return (EXIT_FAILURE);
}
