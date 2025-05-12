/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:04:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/12 20:41:21 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_empty_first_command(t_ast *ast)
{
	char	*first_command;

	if (ast && ast->type == NODE_COMMAND && ast->u_data.s_cmd.argv)
	{
		first_command = get_command_name(ast);
		if (first_command && first_command[0] == '\0')
		{
			ast->type = NODE_EMPTY_COMMAND;
			return (spit_error(CMD_NOT_FOUND, "command not found\n", 0));
		}
	}
	return (-1);
}

static t_token	*process_tokens(const char *cmd, t_scan_status status)
{
	t_token	*tokens;

	if (status != SCAN_SUCCESS)
	{
		printf("%s\n", translate_message(status));
		return (NULL);
	}
	tokens = ft_tokenize(cmd);
	if (!tokens)
		return (NULL);
	tokens = ft_heredoc(tokens);
	return (tokens);
}

static t_ast	*build_ast(t_token *tokens)
{
	t_ast	*ast;

	if (!tokens)
		return (NULL);
	ast = ft_parse(&tokens);
	if (syntax_error(ast, tokens))
		return (NULL);
	return (ast);
}

int	interpreter(const char *cmd)
{
	t_scan_status	status;
	t_token			*tokens;
	t_ast			*ast;
	int				ret_status;
	int				empty_cmd_status;

	status = ft_scan(cmd);
	tokens = process_tokens(cmd, status);
	if (!tokens && status != SCAN_SUCCESS)
		return (SYNTAX_ERROR);
	if (!tokens && status == SCAN_SUCCESS)
		return (SIGINT_EXIT);
	if (!tokens)
		return (SIGINT_EXIT);
	ast = build_ast(tokens);
	if (!ast)
		return (SYNTAX_ERROR);
	empty_cmd_status = handle_empty_first_command(ast);
	if (empty_cmd_status >= 0)
		return (empty_cmd_status);
	ret_status = ft_execute(ast);
	return (ret_status);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;
	init_env_ll(envp);
	setup_shell_signals();
	while (1)
	{
		input = get_command();
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		set_exit_status(interpreter(input));
		free(input);
	}
	free_all();
	return (*get_exit_status());
}
