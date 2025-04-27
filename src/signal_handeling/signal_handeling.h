/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handeling.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 20:53:08 by arajma            #+#    #+#             */
/*   Updated: 2025/04/26 12:12:38 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDELING_H
# define SIGNAL_HANDELING_H

# include <signal.h>
# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <bits/sigaction.h>

void	setup_shell_signals(void);
pid_t	ft_fork(void);
pid_t	ft_waitpid(pid_t pid, int *status, int options);

#endif
