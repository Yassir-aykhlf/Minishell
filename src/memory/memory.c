/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:21:12 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/08 18:12:21 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_allocation	**get_alloc_list(void)
{
	static t_allocation	*alloc_list;

	return (&alloc_list);
}

void	free_all(void)
{
	t_allocation	**alloc_list_ptr;
	t_allocation	*current;
	t_allocation	*next;

	alloc_list_ptr = get_alloc_list();
	current = *alloc_list_ptr;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	*alloc_list_ptr = NULL;
}

void	*ft_malloc(size_t size)
{
	t_allocation	**alloc_list_ptr;
	t_allocation	*new_alloc;
	void			*result;

	alloc_list_ptr = get_alloc_list();
	if (size == 0)
		return (NULL);
	new_alloc = malloc(sizeof(t_allocation));
	if (!new_alloc)
	{
		write(2, "Allocation failed\n", 18);
		free_all();
		exit(EXIT_FAILURE);
	}
	new_alloc->ptr = malloc(size);
	if (!new_alloc->ptr)
	{
		free(new_alloc);
		write(2, "Allocation failed\n", 18);
		free_all();
		exit(EXIT_FAILURE);
	}
	new_alloc->next = *alloc_list_ptr;
	*alloc_list_ptr = new_alloc;
	return (new_alloc->ptr);
}
