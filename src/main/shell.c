/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:04:18 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/10 20:04:30 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

const char *get_token_type_str(t_token_type type)
{
    switch (type)
    {
        case TOKEN_WORD:          return "TOKEN_WORD";
        case TOKEN_PIPE:          return "TOKEN_PIPE";
        case TOKEN_REDIRECT_IN:   return "TOKEN_REDIRECT_IN";
        case TOKEN_REDIRECT_OUT:  return "TOKEN_REDIRECT_OUT";
        case TOKEN_APPEND:        return "TOKEN_APPEND";
        case TOKEN_HEREDOC:       return "TOKEN_HEREDOC";
        case TOKEN_AND:           return "TOKEN_AND";
        case TOKEN_OR:            return "TOKEN_OR";
        case TOKEN_PAREN_OPEN:    return "TOKEN_PAREN_OPEN";
        case TOKEN_PAREN_CLOSE:   return "TOKEN_PAREN_CLOSE";
        case TOKEN_SINGLE_QUOTED: return "TOKEN_SINGLE_QUOTED";
        case TOKEN_DOUBLE_QUOTED: return "TOKEN_DOUBLE_QUOTED";
        default:                  return "UNKNOWN";
    }
}

/* Print each token's type and value in the linked list */
void print_tkn(t_token *tokens)
{
    while (tokens)
    {
        printf("Type: %s, Value: %s\n",
               get_token_type_str(tokens->type),
               tokens->value);
        tokens = tokens->next;
    }
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
		printf("Error: %s\n", translate_message(status));
		return (-1);
	}
	tokens = ft_tokenize(cmnd);
 	if (!tokens)
		return (-1);
	tokens = here_doc(tokens);
	//print_tkn(tokens);
	/* ast = parse(&tokens);
	if (!ast)
		return (-2);
	print_ast(ast);
	//ret_status = ft_execute(ast, envp); */
	return (0/* ret_status */);
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
		free_all();
	}
	return (0);
}
