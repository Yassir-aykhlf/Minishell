/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:38:14 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/12 21:03:20 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <limits.h>

static char	*get_hostname(void)
{
	char	hostname_buf[256];
	
	if (gethostname(hostname_buf, sizeof(hostname_buf)) == -1)
		return (ft_strdup("unknown"));
	
	hostname_buf[sizeof(hostname_buf) - 1] = '\0';
	return (ft_strdup(hostname_buf));
}

static int	is_path_in_home(const char *cwd, const char *home)
{
	size_t	home_len;

	if (!cwd || !home)
		return (0);
	home_len = ft_strlen(home);
	return (ft_strncmp(cwd, home, home_len) == 0
		&& (cwd[home_len] == '/' || cwd[home_len] == '\0'));
}

char	*tilde_implementation(void)
{
	char	*home;
	char	*cwd;
	char	*result;
	size_t	home_len;

	cwd = get_env_value("PWD");
	if (cwd == NULL)
		return (ft_strdup("unknown"));
	home = get_env_value("HOME");
	if (!home)
		return (ft_strdup("unknown"));
	if (is_path_in_home(cwd, home))
	{
		home_len = ft_strlen(home);
		if (cwd[home_len] == '\0')
			result = ft_strdup("~");
		else
			result = ft_strjoin_three("~", "/", cwd + home_len + 1);
	}
	else
		result = ft_strdup(cwd);
	return (result);
}

char	*get_command(void)
{
	char	*cmd;
	char	*cwd;
	char	*user;
	char	*hostname;
	char	*prompt;
	char	*clr_user;
	char	*clr_hostname;
	char	*clr_cwd;

	user = getenv("USER");
	if (!user)
		user = "user";
	clr_user = ft_strjoin_three("\033[1;32m", user, "\033[0m");
	
	hostname = get_hostname();
	if (!hostname)
		hostname = ft_strdup("unknown");
	clr_hostname = ft_strjoin_three("\033[1;34m", hostname, "\033[0m");
	
	cwd = tilde_implementation();
	if (!cwd)
		cwd = ft_strdup("unknown");
	clr_cwd = ft_strjoin_three("\033[1;36m", cwd, "\033[0m");
	
	prompt = ft_strjoin(clr_user, "@");
	prompt = ft_strjoin(prompt, clr_hostname);
	prompt = ft_strjoin(prompt, ":");
	prompt = ft_strjoin(prompt, clr_cwd);
	prompt = ft_strjoin(prompt, "$ ");
	
	cmd = readline(prompt);
	if (!cmd)
		return (NULL);
	if (*cmd)
		add_history(cmd);
	return (cmd);
}

void	syntax_print(char *token)
{
	if (ft_strcmp(token, "syntax error") == 0)
		write(2, "syntax error.\n", 14);
	else
	{
		write(2, "syntax error near unexpected token `", 36);
		write(2, token, ft_strlen(token));
		write(2, "'\n", 2);
	}
}
