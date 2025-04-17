/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:02:24 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/04/16 20:02:02 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

t_env	**get_env_list(void);
void	init_env(char **env);
char	*get_env_value(char *key);
void	set_env(char *key, char *value);
void	unset_env(char *key);
char	**env_to_array(void);
t_env	*create_env_node(char *key, char *value);
void	link_env_node(t_env **head, t_env *node);
void	parse_env_var(const char *var);

#endif
