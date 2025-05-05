/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:38:14 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/05 15:06:46 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <limits.h>

static char	*get_hostname(void)
{
	char	*hostname;
	char	*session_manager;
	int		i;
	int		j;

	session_manager = getenv("SESSION_MANAGER");
	if (!session_manager)
		return (ft_strdup("unknown"));
	i = 0;
	while (session_manager[i])
	{
		if (ft_strncmp(&session_manager[i], "local/", 6) == 0)
			break ;
		i++;
	}
	if (!session_manager[i])
		return (ft_strdup("unknown"));
	i += 6;
	j = 0;
	while (session_manager[i + j] && session_manager[i + j] != '.')
		j++;
	hostname = (char *)ft_malloc(j + 1);
	ft_memcpy(hostname, &session_manager[i], j);
	hostname[j] = '\0';
	return (hostname);
}

char	*tilde_implementation(void)
{
	char	*home;
	char	*cwd;
	char	*result;

	cwd = get_env_value("PWD");
	if (cwd == NULL)
		return (ft_strdup("unknown"));
	home = get_env_value("HOME");
	if (!home)
		return (ft_strdup("unknown"));
	if (ft_strncmp(cwd, home, ft_strlen(home)) == 0
		&& (cwd[ft_strlen(home)] == '/' || cwd[ft_strlen(home)] == '\0'))
	{
		if (cwd[ft_strlen(home)] == '\0')
			result = ft_strdup("~");
		else
			result = ft_strjoin_three("~", "/", cwd + ft_strlen(home) + 1);
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
	char	*prompt;
	char	*hostname;

	user = getenv("USER");
	if (!user)
		user = "user";
	hostname = get_hostname();
	cwd = tilde_implementation();
	prompt = ft_strjoin(user, "@");
	prompt = ft_strjoin(prompt, hostname);
	prompt = ft_strjoin(prompt, ":");
	prompt = ft_strjoin(prompt, cwd);
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
