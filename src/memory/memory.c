/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:21:12 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/05/04 09:48:35 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_alloc	**get_alloc_list(void)
{
	static t_alloc	*alloc_list;

	return (&alloc_list);
}

void	free_all(void)
{
	t_alloc	**alloc_list_ptr;
	t_alloc	*current;
	t_alloc	*next;

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
	t_alloc	**alloc_list_ptr;
	t_alloc	*new_alloc;

	alloc_list_ptr = get_alloc_list();
	if (size == 0)
		return (NULL);
	new_alloc = malloc(sizeof(t_alloc));
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

void	ft_free(void *ptr)
{
	t_alloc	**alloc_list_ptr;
	t_alloc	*current;
	t_alloc	*prev;

	if (!ptr)
		return ;
	alloc_list_ptr = get_alloc_list();
	current = *alloc_list_ptr;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				*alloc_list_ptr = current->next;
			free(current->ptr);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
