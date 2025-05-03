/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:09:33 by arajma            #+#    #+#             */
/*   Updated: 2025/05/03 16:51:43 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../../includes/minishell.h"

typedef struct s_args	t_args;

typedef struct s_expand
{
	t_args	**head;
	t_mask	*mask;
	int		pos;
	int		fs;
	char	*token;
	char	*word;
}	t_expand;

void		expand_command(t_ast *cmd);
void		expand_args_list(t_args **args_head);
void		expand_redirections(t_redir *redirects);
t_args		*expand_token(char *token, t_mask *mask, int fs);
int			handle_field_splitting(t_expand *ex,
				const char *value, int var_start);
t_args		*add_expanded_nodes(t_args *expanded,
				t_args *next, t_args **args_head, t_args *prev);
void		handle_var(t_expand *ex);
void		add_to_list(t_expand *ex);
void		handle_exit_code(t_expand *ex);
char		**ft_split_whitespace(const char *s);
int			contains_whitespace(const char *str);
char		*append_char(char *str, char c);
t_expand	*init_exp_cntext(char *token, t_mask *mask, int fs);
char		*get_var_value(char *name);
char		*get_var_name(t_expand *ex);
int			is_field_split(t_args *arg);
int			is_sstat(t_expand *ex, int start);

#endif
