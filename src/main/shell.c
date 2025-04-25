/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:04:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/25 16:19:51 by yaykhlf          ###   ########.fr       */
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

int	interpreter(const char *cmd)
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
	if (ast && ast->type == NODE_PIPELINE)
	{
		printf("ast->type: %d\n", ast->type);
	}
	if (!ast || tokens)
	{
		if (tokens)
			write(2, "syntax error near unexpected token `newline'\n", 43);
		else
			write(2, "syntax error near unexpected token `;'\n", 39);
		return (2);
	}
	ret_status = ft_execute(ast);
	return (ret_status);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**env;

	(void)argc;
	(void)argv;
	init_env_ll(envp);
	while (1)
	{
		input = get_command();
		interpreter(input);
		free(input);
	}
	free_all();
	return (0);
}
