/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:10:57 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/17 17:23:24 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

bool	is_whitespace(char c);
int		ft_strcmp(const char *s1, const char *s2);
bool	is_operator_char(char c);
char	*ft_strndup(const char *s, size_t n);
int		err_exit(int status, char *message);
char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3);
int		env_listsize(t_env *lst);

#endif
