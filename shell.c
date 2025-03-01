/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:04:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/01 20:56:38 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

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
		{
			free_split(path);
			return (tmp);
		}
		free(tmp);
		i++;
	}
	free_split(path);
	return (NULL);
}

int	ft_execute(t_ast *ast, char *envp[])
{
	// This function executes the given AST.
}

t_ast	*ft_parse(char **tokens)
{
	// This function parses the tokens into an abstract syntax tree (AST).
}

char	**ft_tokenize(const char *cmnd)
{
	// This function tokenizes the input command and returns an array of tokens.
}

/**
 * @brief Orichestrates the tokenization, parsing, and execution of a command.
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
int	ft_spawn(const char *cmnd, char *envp[])
{
	int		status;
	char	**tokens;
	t_ast	*ast;
	
	tokens = ft_tokenize(cmnd); // handle here_doc failure
	if (!tokens)
		return (-1);
	ast = ft_parse(tokens);
	if (!ast)
	{
		free_tokens(tokens);
		return (-2);
	}
	status = ft_execute(ast, envp);
	free_tokens(tokens);
	free_ast(ast);
	return (status);
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
		ft_spawn(cmnd, envp);
		free(cmnd);
	}
	return (0);
}
