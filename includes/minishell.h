/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:05:43 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/17 17:37:12 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/tokenization/tokenize.h"
# include "../src/execution/execute.h"
# include "../src/heredoc/heredoc.h"
# include "../src/environment/env.h"
# include "../src/memory/memory.h"
# include "../src/utils/utils.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "../src/scan/scan.h"
# include "lib/libft/libft.h"
# include "../src/ast/ast.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>

char	*take_command(void);
char	*search_path(char *cmd, char *envp[]);
int		process_command(const char *cmnd, char *envp[]);
t_token	*here_doc(t_token *tokens);
char	*handle_heredoc(char *delim);
t_ast	*ft_parse(t_token **tokens);

#endif
