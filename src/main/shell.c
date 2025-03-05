/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:04:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/05 12:20:53 by yaykhlf          ###   ########.fr       */
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

char	*search_path(char *cmnd, char *envp[])
{
	char	*tmp_slash;
	char	*path_env;
	char	**path;
	char	*tmp;
	int		i;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path_env = envp[i] + 5;
	path = ft_split(path_env, ':');
	if (!path)
	{
		perror("ft_split");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (path[i])
	{
		tmp_slash = ft_strjoin(path[i], "/");
		tmp = ft_strjoin(tmp_slash, cmnd);
		free(tmp_slash);
		if (!access(tmp, X_OK))
			return (tmp);
		i++;
	}
	return (NULL);
}

/**
 * @brief Handles the tokenization, parsing, and execution of a command.
 *
 * This function tokenizes the input command, parses the tokens into an
 * abstract syntax tree (AST), and then executes the AST. It handles
 * memory allocation and deallocation for tokens and the AST.
 *
 * @param cmnd The command to be executed.
 * @param envp The environment variables to be passed to the new process.
 * @return The status code of the executed command. Returns -1 if an error occurs
 * during tokenization, -2 if an error occurs during parsing.
 */
int	process_command(const char *cmnd, char *envp[])
{
	t_scan_status	status;
	t_token			*tokens;
	t_ast			*ast;
	int				ret_status;
	
	status = ft_scanner(cmnd);
	if (status != SCAN_SUCCESS)
	{
		printf("Error: %s\n", scan_status_message(status));
		return (-1);
	}
	tokens = ft_tokenize(cmnd); // handle here_doc failure
	if (!tokens)
		return (-1);
	ast = ft_parse(tokens);
	if (!ast)
	{
		free_tokens(tokens);
		return (-2);
	}
	ret_status = ft_execute(ast, envp);
	free_tokens(tokens);
	free_ast(ast);
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
		process_command(cmnd, envp);
		free(cmnd);
	}
	return (0);
}
