/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:25:50 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/09 23:59:00 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <stdlib.h>

typedef struct s_allocation
{
	void				*ptr;
	struct s_allocation	*next;
}	t_allocation;

t_allocation	**get_alloc_list(void);
void			free_all(void);
void			*ft_malloc(size_t size);

#endif
