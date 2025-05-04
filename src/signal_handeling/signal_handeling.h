/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handeling.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 20:53:08 by arajma            #+#    #+#             */
/*   Updated: 2025/05/04 08:36:17 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDELING_H
# define SIGNAL_HANDELING_H

# include <signal.h>
# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <bits/sigaction.h>
# include <readline/readline.h>

void	setup_shell_signals(void);
void	sigint_handler(int sig);
pid_t	ft_fork(void);
pid_t	ft_waitpid(pid_t pid, int *status, int options);

#endif
