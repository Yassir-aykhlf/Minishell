/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:15:28 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/03 19:18:17 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_child_process(void)
{
	static pid_t	original_pid = 0;

	if (original_pid == 0)
		original_pid = getpid();
	return (getpid() != original_pid);
}

void	fill_argv_from_list(char **argv, t_args *args)
{
	int	i;

	i = 0;
	while (args)
	{
		argv[i++] = args->arg;
		args = args->next;
	}
	argv[i] = NULL;
}

char	**get_argv(t_args *args)
{
	int		arg_count;
	char	**argv;

	arg_count = count_args(args);
	argv = ft_malloc(sizeof(char *) * (arg_count + 1));
	if (!argv)
	{
		spit_error(EXIT_FAILURE, "ft_malloc", true);
		return (NULL);
	}
	fill_argv_from_list(argv, args);
	return (argv);
}

int	count_args(t_args *args)
{
	int	count;

	count = 0;
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}
