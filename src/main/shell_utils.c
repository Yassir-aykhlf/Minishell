/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:38:14 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 18:38:47 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_command(void)
{
	char	*cmd;

	cmd = readline("\033[92mEnigma\033[94m$ \033[0m");
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
