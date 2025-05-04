/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:25:50 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 13:43:52 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**handle_echo_newline_flag(char **args, int *newline)
{
	int	i;
	int	j;

	*newline = 1;
	i = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break;
		*newline = 0;
		i++;
	}
	return (args + i - 1);
}

void	print_echo_args(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

int	builtin_echo(char **args)
{
	int		newline;
	char	**args_to_print;

	args_to_print = handle_echo_newline_flag(args, &newline);
	print_echo_args(args_to_print);
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
