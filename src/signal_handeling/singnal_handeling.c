/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singnal_handeling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 20:54:15 by arajma            #+#    #+#             */
/*   Updated: 2025/04/30 15:54:51 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	return (pid);
}

void	sigint_handler(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_shell_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

pid_t	ft_waitpid(pid_t pid, int *status, int options)
{
	int					sig;
	pid_t				result;
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct sigaction	temp_act;

	temp_act.sa_handler = SIG_IGN;
	sigemptyset(&temp_act.sa_mask);
	temp_act.sa_flags = 0;
	sigaction(SIGINT, &temp_act, &old_int);
	sigaction(SIGQUIT, &temp_act, &old_quit);
	result = waitpid(pid, status, options);
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	if (result > 0 && WIFSIGNALED(*status))
	{
		sig = WTERMSIG(*status);
		if (sig == SIGINT)
			printf("\n");
		else if (sig == SIGQUIT)
			printf("Quit (core dumped)\n");
	}
	return (result);
}
