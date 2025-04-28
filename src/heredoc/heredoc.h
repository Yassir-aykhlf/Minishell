/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:54:57 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/28 11:46:34 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "../../src/ast/ast.h"

char	*handle_heredoc(t_token *tokens);
t_token	*ft_heredoc(t_token *tokens);
char	*expand_heredoc(char *input);
char	*get_var_value_hd(char *var_name);
int		get_var_name_hd(char *str, char **var_name);
int		to_expand(char *mask);

#endif
