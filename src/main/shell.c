/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:04:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/23 23:10:48 by yaykhlf          ###   ########.fr       */
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

int	interpret(const char *cmnd, char *envp[])
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
			printf("syntax error near unexpected token `%s'\n", tokens->value);
		else
			printf("syntax error unexpected token `%s'\n", last_token(ft_tokenize(cmnd)));
		return (2);
	}
	// print_ast(ast); 
	ret_status = ft_execute(ast, envp);
	return (ret_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*cmnd;

	(void)argc;
	(void)argv;
	while (1)
	{
		cmnd = take_command();
		if (!cmnd || !ft_strcmp(cmnd, "exit"))
			break ;
		interpret(cmnd, envp);
		// free_all();
		// free(cmnd);
	}
	return (0);
}
