/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:04:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/17 16:01:05 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*take_command(void)
{
	char	*cmnd;

	cmnd = readline("\033[92mEnigma\033[94m$ \033[0m");
	add_history(cmnd);
	return (cmnd);
}

char	*last_token(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current->next)
		current = current->next;
	return (current->value);
}

int	interpreter(const char *cmnd, char *env[])
{
	t_scan_status	status;
	t_token			*tokens;
	t_ast			*ast;
	int				ret_status;
	
	status = ft_scan(cmnd);
	if (status != SCAN_SUCCESS)
	{
		printf("Error: %s\n", translate_message(status));
		return (-1);
	}
	tokens = ft_tokenize(cmnd);
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
	// print_ast(ast); 
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

int	main(int argc, char *argv[], char *envp[])
{
	char	*cmnd;
	char	**env;

	(void)argc;
	(void)argv;
	init_env(envp);
	env = env_to_array();
	while (1)
	{
		cmnd = take_command();
		if (!cmnd || !ft_strcmp(cmnd, "exit"))
			break ;
		interpreter(cmnd, env);
		// free_all();
		// free(cmnd);
	}
	return (0);
}
