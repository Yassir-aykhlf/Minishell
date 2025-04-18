/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:04:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/18 15:52:05 by yaykhlf          ###   ########.fr       */
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

char	*last_token(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current->next)
		current = current->next;
	return (current->value);
}

int	interpreter(const char *cmd, char *env[])
{
	t_scan_status	status;
	t_token			*tokens;
	t_ast			*ast;
	int				ret_status;
	
	status = ft_scan(cmd);
	if (status != SCAN_SUCCESS)
	{
		printf("Error: %s\n", translate_message(status));
		return (-1);
	}
	tokens = ft_tokenize(cmd);
 	if (!tokens)
		return (-1);
	tokens = ft_heredoc(tokens);
	ast = ft_parse(&tokens);
	if (!ast || tokens)
	{
		if (tokens)
			write(2, "syntax error near unexpected token `newline'\n", 43);
		else
			write(2, "syntax error near unexpected token `;'\n", 39);
		return (2);
	}
	ret_status = ft_execute(ast, env);
	return (ret_status);
}

void	print_env(char *env[])
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**env;

	(void)argc;
	(void)argv;
	init_env_ll(envp);
	env = env_to_array();
	while (1)
	{
		input = get_command();
		interpreter(input, env);
		free_all();
		free(input);
	}
	return (0);
}
