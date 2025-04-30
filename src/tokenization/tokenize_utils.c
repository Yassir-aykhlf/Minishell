/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 21:06:48 by arajma            #+#    #+#             */
/*   Updated: 2025/04/30 21:16:30 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	get_quote(char quote)
{
	if (quote == '\'')
		return ('S');
	return ('D');
}

void	skip_whitespace(const char *input, int *pos)
{
	while (input[*pos] && is_whitespace(input[*pos]))
		(*pos)++;
}

size_t	find_max_segment_id(const t_mask *mask, size_t length)
{
	size_t(max_id) = 0;
	size_t(i) = 0;
	while (i < length)
	{
		if (mask->seg[i] > max_id)
			max_id = mask->seg[i];
		i++;
	}
	return (max_id);
}

size_t	*create_merged_segments(const t_mask *mask1, const t_mask *mask2)
{
	int (i) = 0;
	size_t (mask1_len) = ft_strlen(mask1->mask);
	size_t (mask2_len) = ft_strlen(mask2->mask);
	size_t (max_seg_id);
	size_t (total_len) = mask1_len + mask2_len;
	size_t *(merged_segments) = ft_malloc(sizeof(size_t) * total_len);
	while (i < mask1_len)
	{
		merged_segments[i] = mask1->seg[i];
		i++;
	}
	max_seg_id = find_max_segment_id(mask1, mask1_len);
	i = 0;
	while (i < mask2_len)
	{
		merged_segments[mask1_len + i] = mask2->seg[i] + max_seg_id + 1;
		i++;
	}
	return (merged_segments);
}

t_mask	*merge_masks(t_mask *mask1, t_mask *mask2)
{
	t_mask	*new_mask;
	char	*new_mask_str;
	size_t	*new_segments;
	size_t	total_len;

	if (!mask1 || !mask2)
		return (NULL);
	total_len = ft_strlen(mask1->mask) + ft_strlen(mask2->mask);
	new_mask_str = ft_strjoin(mask1->mask, mask2->mask);
	new_segments = create_merged_segments(mask1, mask2);
	new_mask = create_mask(new_mask_str, new_segments, total_len);
	return (new_mask);
}
