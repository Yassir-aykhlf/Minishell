/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:19:30 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 17:49:05 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*get_exit_status(void)
{
	static int	exit_status = 0;

	return (&exit_status);
}

void	set_exit_status(int status)
{
	int	*exit_status_ptr;

	exit_status_ptr = get_exit_status();
	*exit_status_ptr = status;
}

int	*get_var_expansion_flag(void)
{
	static int	var_expansion_flag = 0;

	return (&var_expansion_flag);
}

void	set_var_expansion_flag(int flag)
{
	int	*var_flag_ptr;

	var_flag_ptr = get_var_expansion_flag();
	*var_flag_ptr = flag;
}
