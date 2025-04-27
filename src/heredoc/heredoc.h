/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:54:57 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/27 14:54:48 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "../../src/ast/ast.h"

char	*handle_heredoc(char *delim);
t_token	*ft_heredoc(t_token *tokens);
char	*expand_heredoc(char *input);
char	*get_var_value_hd(char *var_name);

#endif
