/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:54:57 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/13 14:56:37 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "../../src/ast/ast.h"

int		ft_swap_nodes(t_token *node1, t_token *node2);
char	*handle_heredoc(char *delim);
t_token	*here_doc(t_token *tokens);

#endif
